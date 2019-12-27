// Copyright (c) 2014, Thomas Goyne <plorkyeran@aegisub.org>
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
// Aegisub Project http://www.aegisub.org/

#include "video_provider_manager.h"

#include "autoreghook.h"
#include "include/aegisub/video_provider.h"
#include "options.h"

#include <libaegisub/fs.h>
#include <libaegisub/log.h>

std::unique_ptr<VideoProvider> CreateDummyVideoProvider(agi::fs::path const&, std::string const&, agi::BackgroundRunner *);
std::unique_ptr<VideoProvider> CreateYUV4MPEGVideoProvider(agi::fs::path const&, std::string const&, agi::BackgroundRunner *);
std::unique_ptr<VideoProvider> CreateFFmpegSourceVideoProvider(agi::fs::path const&, std::string const&, agi::BackgroundRunner *);
std::unique_ptr<VideoProvider> CreateAvisynthVideoProvider(agi::fs::path const&, std::string const&, agi::BackgroundRunner *);

std::unique_ptr<VideoProvider> CreateCacheVideoProvider(std::unique_ptr<VideoProvider>);

namespace video_provider {
agi::registry<Factory> _registry;

std::vector<std::string> GetClasses() {
	return _registry.get_entries_names();
}

std::unique_ptr<VideoProvider> GetProvider(agi::fs::path const& filename, std::string const& colormatrix, agi::BackgroundRunner* br) {
	auto preferred = OPT_GET("Video/Provider")->GetString();
	auto sorted = std::set<Factory*, agi::factory_comparator>(agi::factory_comparator{ preferred.c_str() });
	for (auto& entry : _registry)
		sorted.insert(entry.second.get());

	bool found = false;
	bool supported = false;
	std::string errors;
	errors.reserve(1024);

	for (auto factory : sorted) {
		std::string err;
		try {
			auto provider = factory->create(filename, colormatrix, br);
			if (!provider) continue;
			LOG_I("manager/video/provider") << factory->name << ": opened " << filename;
			return provider->WantsCaching() ? CreateCacheVideoProvider(std::move(provider)) : std::move(provider);
		}
		catch (agi::fs::FileNotFound const&) {
			err = "file not found.";
			// Keep trying other providers as this one may just not be able to
			// open a valid path
		}
		catch (VideoNotSupported const&) {
			found = true;
			err = "video is not in a supported format.";
		}
		catch (VideoOpenError const& ex) {
			supported = true;
			err = ex.GetMessage();
		}
		catch (agi::vfr::Error const& ex) {
			supported = true;
			err = ex.GetMessage();
		}

		errors += std::string(factory->name) + ": " + err + "\n";
		LOG_D("manager/video/provider") << factory->name << ": " << err;
	}

	// No provider could open the file
	LOG_E("manager/video/provider") << "Could not open " << filename;
	std::string msg = "Could not open " + filename.string() + ":\n" + errors;

	if (!found) throw agi::fs::FileNotFound(filename.string());
	if (!supported) throw VideoNotSupported(msg);
	throw VideoOpenError(msg);
}

agi::registry<Factory>& GetRegistry()
{
	return _registry;
}

START_HOOK_BEGIN(videoProvider)
#define DEFINE_VIDEO_PROVIDER(name, func, hidden) _registry.register_entry(#name, std::make_unique<Factory>(std::string(#name), func, hidden ))
DEFINE_VIDEO_PROVIDER(Dummy, CreateDummyVideoProvider, true);
DEFINE_VIDEO_PROVIDER(YUV4MPEG, CreateYUV4MPEGVideoProvider, true);
#ifdef WITH_FFMS2
DEFINE_VIDEO_PROVIDER(FFmpegSource, CreateFFmpegSourceVideoProvider, false);
#endif
#ifdef WITH_AVISYNTH
DEFINE_AUDIO_PROVIDER(Avisynth, CreateAvisynthVideoProvider, false);
#endif
START_HOOK_END
}
