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

// INTERNAL USE
// Use forward decl to use those functions.

#include "include/aegisub/menu_initializer.h"

#include "menu_dynamic_builtin.h"
#include "compat.h"

#include <vector>
#include <string>

std::vector<menu::MenuNode> GetMainMenuWinLinux(agi::Context* context);
std::vector<menu::MenuNode> GetMainMenuMac(agi::Context* context);

std::vector<menu::MenuNode> GetMainMenu(agi::Context* context)
{
#ifdef __WXMAC__
    return GetMainMenuMac(context);
#else
    return GetMainMenuWinLinux(context);
#endif
}

std::vector<menu::MenuNode> GetMainMenuWinLinux(agi::Context* context)
{
    return std::vector<menu::MenuNode> {
        {
            GetTranslation_std("&File"), {
                { "subtitle/new" },
                { "subtitle/open" },
                { "subtitle/open/charset" },
                { "subtitle/open/video" },
                { "subtitle/open/autosave" },
                { "subtitle/save" },
                { "subtitle/save/as" },
                { "tool/export" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Subtitle", context) },
                {},
                { "subtitle/properties" },
                { "subtitle/attachment" },
                { "tool/font_collector" },
                {},
                { "app/new_window" },
                { "app/exit", menu::SpecialMenuNode::exit },
            }
        },
        {
            GetTranslation_std("&Edit"), {
                { "edit/undo" },
                { "edit/redo" },
                {},
                { "edit/line/cut" },
                { "edit/line/copy" },
                { "edit/line/paste" },
                { "edit/line/paste/over" },
                {},
                { "subtitle/find" },
                { "subtitle/find/next" },
                { "edit/find_replace" },
            }
        },
        {
            GetTranslation_std("&Subtitle"), {
                { "tool/style/manager" },
                { "tool/style/assistant" },
                { "tool/translation_assistant" },
                { "tool/resampleres" },
                { "subtitle/spellcheck" },
                {},
                { "tool/assdraw" },
                {
                    GetTranslation_std("&Insert Lines"), {
                        { "subtitle/insert/before" },
                        { "subtitle/insert/after" },
                        { "subtitle/insert/before/videotime" },
                        { "subtitle/insert/after/videotime" },
                    }
                },
                { "edit/line/duplicate" },
                { "edit/line/split/before" },
                { "edit/line/split/after" },
                { "edit/line/delete" },
                {},
                {
                    GetTranslation_std("Join Lines"), {
                        { "edit/line/join/concatenate" },
                        { "edit/line/join/keep_first" },
                        { "edit/line/join/as_karaoke" },
                    }
                },
                { "edit/line/recombine" },
                { "edit/line/split/by_karaoke" },
                {},
                {
                    GetTranslation_std("Sort All Lines"), {
                        { "grid/sort/start" },
                        { "grid/sort/end" },
                        { "grid/sort/style" },
                        { "grid/sort/actor" },
                        { "grid/sort/effect" },
                        { "grid/sort/layer" },
                    }
                },
                {
                    GetTranslation_std("Sort Selected Lines"), {
                        { "grid/sort/start/selected" },
                        { "grid/sort/end/selected" },
                        { "grid/sort/style/selected" },
                        { "grid/sort/actor/selected" },
                        { "grid/sort/effect/selected" },
                        { "grid/sort/layer/selected" },
                    }
                },
                { "grid/swap" },
                { "tool/line/select" },
                { "subtitle/select/all" },
            }
        },
        {
            GetTranslation_std("&Timing"), {
                { "time/shift" },
                { "tool/time/postprocess" },
                { "tool/time/kanji" },
                {},
                { "time/snap/start_video" },
                { "time/snap/end_video" },
                { "time/snap/scene" },
                { "time/frame/current" },
                {},
                {
                    GetTranslation_std("Make Times Continuous"), {
                        { "time/continuous/start" },
                        { "time/continuous/end" },
                    }
                },
            }
        },
        {
            GetTranslation_std("&Video"), {
                { "video/open" },
                { "video/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Video", context) },
                { "video/open/dummy" },
                { "video/details" },
                {},
                { "timecode/open" },
                { "timecode/save" },
                { "timecode/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Timecodes", context) },
                {},
                { "keyframe/open" },
                { "keyframe/save" },
                { "keyframe/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Keyframes", context) },
                {},
                { "video/detach" },
                {
                    GetTranslation_std("Set &Zoom"), {
                        { "video/zoom/50" },
                        { "video/zoom/100" },
                        { "video/zoom/200" },
                    }
                },
                {
                    GetTranslation_std("Override &AR"), {
                        { "video/aspect/default" },
                        { "video/aspect/full" },
                        { "video/aspect/wide" },
                        { "video/aspect/cinematic" },
                        { "video/aspect/custom" },
                    }
                },
                { "video/show_overscan" },
                {},
                { "video/jump" },
                { "video/jump/start" },
                { "video/jump/end" },
            }
        },
        {
            GetTranslation_std("&Audio"), {
                { "audio/open" },
                { "audio/open/video" },
                { "audio/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Audio", context) },
                {},
                { "audio/view/spectrum" },
                { "audio/view/waveform" },
                { "audio/open/blank" },
                { "audio/open/noise" },
            }
        },
        //{ GetTranslation_std("A&utomation"), new menu::AutomationMenu(context) },
        {
            GetTranslation_std("Vie&w"), {
                { "app/language" },
                { "app/options", menu::SpecialMenuNode::options },
                {},
                { "app/display/subs" },
                { "app/display/video_subs" },
                { "app/display/audio_subs" },
                { "app/display/full" },
                {},
                { "grid/tags/show" },
                { "grid/tags/simplify" },
                { "grid/tags/hide" },
                {},
                { "app/toggle/toolbar" },
            }
        },
        {
            GetTranslation_std("&Help"), {
                { "help/contents" },
                {},
                { "help/website" },
                { "help/forums" },
                { "help/bugs" },
                {},
                { "help/irc" },
                { "app/updates" },
                { "app/about", menu::SpecialMenuNode::about },
                { "app/log" },
            }
        }
    };
}

std::vector<menu::MenuNode> GetMainMenuMac(agi::Context* context)
{
    return std::vector<menu::MenuNode> {
        {
            GetTranslation_std("&File"), {
                { "subtitle/new", "New" },
                { "subtitle/open", "Open..." },
                { "subtitle/open/charset" },
                { "subtitle/open/video" },
                { "subtitle/open/autosave" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Subtitle", context) },
                {},
                { "subtitle/close" },
                { "subtitle/save", "Save" },
                { "subtitle/save/as", "Save As..." },
                { "tool/export", "Export As..." },
                {},
                { "subtitle/properties" },
                { "subtitle/attachment" },
                { "tool/font_collector" },
                {},
                { "app/exit", menu::SpecialMenuNode::exit },
            }
        },
        {
            GetTranslation_std("&Edit"), {
                { "edit/undo" },
                { "edit/redo" },
                {},
                { "edit/line/cut" },
                { "edit/line/copy" },
                { "edit/line/paste" },
                { "edit/line/paste/over" },
                { "subtitle/select/all" },
                {},
                { "subtitle/find" },
                { "subtitle/find/next" },
                { "edit/find_replace" },
                { "subtitle/spellcheck" },
            }
        },
        {
            GetTranslation_std("Vie&w"), {
                { "app/language" },
                { "app/options", menu::SpecialMenuNode::options },
                {},
                { "app/display/subs" },
                { "app/display/video_subs" },
                { "app/display/audio_subs" },
                { "app/display/full" },
                {},
                { "grid/tags/show" },
                { "grid/tags/simplify" },
                { "grid/tags/hide" },
                {},
                { "app/toggle/toolbar" },
            }
        },
        {
            GetTranslation_std("&Subtitle"), {
                { "tool/style/manager" },
                { "tool/style/assistant" },
                { "tool/translation_assistant" },
                { "tool/resampleres" },
                {},
                {
                    GetTranslation_std("&Insert Lines"), {
                        { "subtitle/insert/before" },
                        { "subtitle/insert/after" },
                        { "subtitle/insert/before/videotime" },
                        { "subtitle/insert/after/videotime" },
                    }
                },
                { "edit/line/duplicate" },
                { "edit/line/split/before" },
                { "edit/line/split/after" },
                { "edit/line/delete" },
                {},
                {
                    GetTranslation_std("Join Lines"), {
                        { "edit/line/join/concatenate" },
                        { "edit/line/join/keep_first" },
                        { "edit/line/join/as_karaoke" },
                    }
                },
                { "edit/line/recombine" },
                { "edit/line/split/by_karaoke" },
                {},
                {
                    GetTranslation_std("Sort All Lines"), {
                        { "grid/sort/start" },
                        { "grid/sort/end" },
                        { "grid/sort/style" },
                        { "grid/sort/actor" },
                        { "grid/sort/effect" },
                        { "grid/sort/layer" },
                    }
                },
                {
                    GetTranslation_std("Sort Selected Lines"), {
                        { "grid/sort/start/selected" },
                        { "grid/sort/end/selected" },
                        { "grid/sort/style/selected" },
                        { "grid/sort/actor/selected" },
                        { "grid/sort/effect/selected" },
                        { "grid/sort/layer/selected" },
                    }
                },
                { "grid/swap" },
                { "tool/line/select" },
            }
        },
        {
            GetTranslation_std("&Timing"), {
                { "time/shift" },
                { "tool/time/postprocess" },
                { "tool/time/kanji" },
                {},
                { "time/snap/start_video" },
                { "time/snap/end_video" },
                { "time/snap/scene" },
                { "time/frame/current" },
                {},
                {
                    GetTranslation_std("Make Times Continuous"), {
                        { "time/continuous/start" },
                        { "time/continuous/end" },
                    }
                },
            }
        },
        {
            GetTranslation_std("&Video"), {
                { "video/open" },
                { "video/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Video", context) },
                { "video/open/dummy" },
                { "video/details" },
                {},
                { "timecode/open" },
                { "timecode/save" },
                { "timecode/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Timecodes", context) },
                {},
                { "keyframe/open" },
                { "keyframe/save" },
                { "keyframe/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Keyframes", context) },
                {},
                { "video/detach" },
                {
                    GetTranslation_std("Set &Zoom"), {
                        { "video/zoom/50" },
                        { "video/zoom/100" },
                        { "video/zoom/200" },
                    }
                },
                {
                    GetTranslation_std("Override &AR"), {
                        { "video/aspect/default" },
                        { "video/aspect/full" },
                        { "video/aspect/wide" },
                        { "video/aspect/cinematic" },
                        { "video/aspect/custom" },
                    }
                },
                { "video/show_overscan" },
                {},
                { "video/jump" },
                { "video/jump/start" },
                { "video/jump/end" },
            }
        },
        {
            GetTranslation_std("&Audio"), {
                { "audio/open" },
                { "audio/open/video" },
                { "audio/close" },
                { GetTranslation_std("&Recent"), new menu::MruMenu("Audio", context) },
                {},
                { "audio/view/spectrum" },
                { "audio/view/waveform" },
                { "audio/open/blank" },
                { "audio/open/noise" },
            }
        },
        //{ GetTranslation_std("A&utomation"), new menu::AutomationMenu(context) },
        {
            GetTranslation_std("Window"), {
                { "app/minimize" },
                { "app/maximize" },
                {},
                { "app/bring_to_front" },
                {},
            }, menu::SpecialMenuNode::window
        },
        {
            GetTranslation_std("&Help"), {
                { "help/contents" },
                {},
                { "help/website" },
                { "help/forums" },
                { "help/bugs" },
                {},
                { "help/irc" },
                { "app/updates" },
                { "app/about", menu::SpecialMenuNode::about },
                { "app/log" },
            }
        }
    };
}

std::vector<menu::MenuNode> GetVideoContextMenu(agi::Context*)
{
    return std::vector<menu::MenuNode> {
        { "video/frame/save" },
        { "video/frame/copy" },
        {},
        { "video/frame/save/raw" },
        { "video/frame/copy/raw" },
        {},
        { "video/copy_coordinates" }
    };
}

std::vector<menu::MenuNode> GetGridContextMenu(agi::Context*)
{
    return std::vector<menu::MenuNode>  {
        { "subtitle/insert/before", "&Insert (before)" },
        { "subtitle/insert/after", "Insert (after)" },
        { "subtitle/insert/before/videotime", "Insert at video time (before)" },
        { "subtitle/insert/after/videotime", "Insert at video time (after)" },
        {},
        { "edit/line/duplicate" },
        { "edit/line/split/before" },
        { "edit/line/split/after" },
        {},
        { "grid/swap" },
        { "edit/line/join/concatenate", "&Join (concatenate)" },
        { "edit/line/join/keep_first", "Join (keep first)" },
        { "edit/line/join/as_karaoke", "Join (as Karaoke)" },
        {},
        { "time/continuous/start", "&Make times continuous (change start)" },
        { "time/continuous/end", "&Make times continuous (change end)" },
        { "edit/line/recombine" },
        {},
        { "audio/save/clip" },
        {},
        { "edit/line/cut" },
        { "edit/line/copy" },
        { "edit/line/paste" },
        { "edit/line/paste/over" },
        {},
        { "edit/line/delete" }
    };
}
