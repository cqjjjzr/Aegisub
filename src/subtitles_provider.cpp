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

#include "include/aegisub/subtitles_provider.h"

#include "ass_dialogue.h"
#include "ass_attachment.h"
#include "ass_file.h"
#include "ass_info.h"
#include "ass_style.h"
#include "factory_manager.h"
#include "options.h"
#include "subtitles_provider_csri.h"
#include "subtitles_provider_libass.h"
#include "autoreghook.h"

namespace subtitles_provider {
agi::registry<Factory> _registry;

std::vector<std::string> GetClasses() {
	return _registry.get_entries_names();
}

std::unique_ptr<SubtitlesProvider> GetProvider(agi::BackgroundRunner* br) {
	auto preferred = OPT_GET("Subtitle/Provider")->GetString();
	auto sorted = std::set<Factory*, agi::factory_comparator>(agi::factory_comparator{ preferred.c_str() });
	for (auto& entry : _registry)
		sorted.insert(entry.second.get());

	std::string error;
	for (auto factory : sorted) {
		try {
			auto provider = factory->create(br);
			if (provider) return provider;
		}
		catch (agi::UserCancelException const&) { throw; }
		catch (agi::Exception const& err) { error += factory->name + ": " + err.GetMessage() + "\n"; }
		catch (...) { error += factory->name + ": Unknown error\n"; }
	}

	throw error;
}

/*std::vector<factory> const& factories() {
	static std::vector<factory> factories;
	if (factories.size()) return factories;
#ifdef WITH_CSRI
		for (auto const& subtype : csri::List())
			factories.push_back(factory{"CSRI/" + subtype, subtype, csri::Create, false});
#endif
		factories.push_back(factory{"libass", "", libass::Create, false});
		return factories;
	}*/
START_HOOK_BEGIN(subtitlesProvider)
#define DEFINE_SUBTITLES_PROVIDER(name, func, hidden) _registry.register_entry(#name, std::make_unique<Factory>(std::string(#name), func, hidden ))
#ifdef WITH_CSRI
for (auto const& subtype : csri::List())
_registry.register_entry(
	"CSRI/" + subtype,
	std::make_unique<Factory>(
		    std::string("CSRI/" + subtype),
		    [&](agi::BackgroundRunner* br) -> std::unique_ptr<SubtitlesProvider> { return libass::Create(subtype, br); }, false));
#endif
DEFINE_SUBTITLES_PROVIDER(libass, [&](agi::BackgroundRunner* br) -> std::unique_ptr<SubtitlesProvider> { return libass::Create("", br); }, false);
START_HOOK_END
}

void SubtitlesProvider::LoadSubtitles(AssFile* subs, int time) {
	buffer.clear();

	auto push_header = [&](const char* str) {
		buffer.insert(buffer.end(), str, str + strlen(str));
	};
	auto push_line = [&](std::string const& str) {
		buffer.insert(buffer.end(), &str[0], &str[0] + str.size());
		buffer.push_back('\n');
	};

	push_header("\xEF\xBB\xBF[Script Info]\n");
	for (auto const& line : subs->Info)
		push_line(line.GetEntryData());

	push_header("[V4+ Styles]\n");
	for (auto const& line : subs->Styles)
		push_line(line.GetEntryData());

	if (!subs->Attachments.empty()) {
		// TODO: some scripts may have a lot of attachments,
		// so ideally we'd want to write only those actually used on the requested video frame,
		// but this would require some pre-parsing of the attached font files with FreeType,
		// which isn't probably trivial.
		push_header("[Fonts]\n");
		for (auto const& attachment : subs->Attachments)
			if (attachment.Group() == AssEntryGroup::FONT)
				push_line(attachment.GetEntryData());
	}

	push_header("[Events]\n");
	for (auto const& line : subs->Events) {
		if (!line.Comment && (time < 0 || !(line.Start > time || line.End <= time)))
			push_line(line.GetEntryData());
	}

	LoadSubtitles(&buffer[0], buffer.size());
}
