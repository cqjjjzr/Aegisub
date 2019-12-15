// Copyright (c) 2005-2007, Rodrigo Braz Monteiro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Aegisub Group nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Aegisub Project http://www.aegisub.org/

/// @file audio_player.cpp
/// @brief Baseclass for audio players
/// @ingroup audio_output
///

#include "include/aegisub/audio_player.h"

#include "audio_controller.h"
#include "options.h"
#include "autoreghook.h"

#include <boost/range/iterator_range.hpp>

#ifdef WITH_ALSA
std::unique_ptr<AudioPlayer> CreateAlsaPlayer(agi::AudioProvider *providers, wxWindow *window);
#endif
#ifdef WITH_DIRECTSOUND
std::unique_ptr<AudioPlayer> CreateDirectSoundPlayer(agi::AudioProvider *providers, wxWindow *window);
std::unique_ptr<AudioPlayer> CreateDirectSound2Player(agi::AudioProvider *providers, wxWindow *window);
#endif
#ifdef WITH_OPENAL
std::unique_ptr<AudioPlayer> CreateOpenALPlayer(agi::AudioProvider *providers, wxWindow *window);
#endif
#ifdef WITH_PORTAUDIO
std::unique_ptr<AudioPlayer> CreatePortAudioPlayer(agi::AudioProvider *providers, wxWindow *window);
#endif
#ifdef WITH_LIBPULSE
std::unique_ptr<AudioPlayer> CreatePulseAudioPlayer(agi::AudioProvider *providers, wxWindow *window);
#endif
#ifdef WITH_OSS
std::unique_ptr<AudioPlayer> CreateOSSPlayer(agi::AudioProvider *providers, wxWindow *window);
#endif

namespace
{
agi::registry<AudioPlayerFactory> _registry;
}

std::vector<std::string> AudioPlayerManager::GetNames() {
	return _registry.get_entries_names();
}

std::unique_ptr<AudioPlayer> AudioPlayerManager::Create(agi::AudioProvider *provider, wxWindow *window) {
	if (_registry.empty())
		throw AudioPlayerOpenError("No audio players are available.");

	auto preferred = OPT_GET("Audio/Player")->GetString();
	auto sorted = std::set<AudioPlayerFactory*, agi::factory_comparator>(agi::factory_comparator{ preferred.c_str() });
	for (auto& entry : _registry)
		sorted.insert(entry.second.get());

	std::string error;
	for (auto& factory : sorted) {
		try {
			return factory->create(provider, window);
		}
		catch (AudioPlayerOpenError const& err) {
			error += std::string(factory->name) + " factory: " + err.GetMessage() + "\n";
		}
	}
	throw AudioPlayerOpenError(error);
}

agi::registry<AudioPlayerFactory>& AudioPlayerManager::GetRegistry()
{
	return _registry;
}

START_HOOK_BEGIN(audioPlayer)
#define DEFINE_AUDIO_PLAYER(name, func) _registry.register_entry(#name, std::make_unique<AudioPlayerFactory>(std::string(#name), func, false ))

#ifdef WITH_ALSA
    DEFINE_AUDIO_PLAYER(ALSA, CreateAlsaPlayer);
#endif
#ifdef WITH_DIRECTSOUND
    DEFINE_AUDIO_PLAYER(DirectSound-old, CreateDirectSoundPlayer);
    DEFINE_AUDIO_PLAYER(DirectSound, CreateDirectSound2Player);
#endif
#ifdef WITH_OPENAL
	DEFINE_AUDIO_PLAYER(OpenAL, CreateOpenALPlayer);
#endif
#ifdef WITH_PORTAUDIO
	DEFINE_AUDIO_PLAYER(PortAudio, CreatePortAudioPlayer);
#endif
#ifdef WITH_LIBPULSE
		DEFINE_AUDIO_PLAYER(PulseAudio, CreatePulseAudioPlayer);
#endif
#ifdef WITH_OSS
		DEFINE_AUDIO_PLAYER(OSS, CreateOSSPlayer);
#endif
START_HOOK_END
