// Copyright (c) 2019, Charlie Jiang <cqjjjzr@126.com>
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

#include <libaegisub/mru.h>
#include "autoreghook.h"
#include "include/aegisub/context.h"
#include "project.h"
#include "subs_controller.h"

using namespace agi::mru;

namespace
{
MRUDescriptor audioDesc{ "Audio",        [](agi::Context* c, auto& path) -> void { c->project->LoadAudio(path); }, "Limits/MRU" };
MRUDescriptor videoDesc{ "Video",        [](agi::Context* c, auto& path) -> void { c->project->LoadVideo(path); }, "Limits/MRU" };
MRUDescriptor keyframesDesc{ "Keyframes",[](agi::Context* c, auto& path) -> void { c->project->LoadKeyframes(path); }, "Limits/MRU" };
MRUDescriptor subtitleDesc{ "Subtitle",  [](agi::Context* c, auto& path) -> void
{
	#ifdef __APPLE__
		wxGetApp().NewProjectContext().project->LoadSubtitles(path);
#else
		if (c->subsController->TryToClose() == wxCANCEL) return;
		c->project->LoadSubtitles(path);
#endif
}, "Limits/MRU" };
MRUDescriptor timecodesDesc{ "Timecodes",[](agi::Context* c, auto& path) -> void { c->project->LoadTimecodes(path); }, "Limits/MRU" };
MRUDescriptor findDesc{ "Find",          [](agi::Context*, auto&) -> void {}, "Limits/Find Replace" };       // No-op handler
MRUDescriptor replaceDesc{ "Replace",    [](agi::Context*, auto&) -> void {}, "Limits/Find Replace" };
}

START_HOOK_BEGIN(mruBuiltin)
RegisterMRU(audioDesc);
RegisterMRU(findDesc);
RegisterMRU(keyframesDesc);
RegisterMRU(replaceDesc);
RegisterMRU(subtitleDesc);
RegisterMRU(timecodesDesc);
RegisterMRU(videoDesc);
START_HOOK_END