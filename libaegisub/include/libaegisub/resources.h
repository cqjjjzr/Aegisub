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

/// @file resources.h
/// @brief Public interface for resource(bitmap, config, ...) loading.
/// @ingroup libaegisub

#pragma once

#include <string>
#include <string_view>

#include <boost/optional/optional.hpp>
#include <wx/bitmap.h>
#include <wx/icon.h>

#include "libaegisub/resources_fs.h"

namespace agi::resources
{
wxBitmap LoadImageResInternal(std::string_view name, int dir = 0);
wxIcon LoadIconResInternal(std::string_view name);
std::string LoadConfigInternal(std::string_view name);


/// Load image resource using name.
/// @param dir The image direction
inline wxBitmap LoadImageRes(std::string_view name, int dir = 0)
{
    if (boost::optional<wxBitmap> fsResource = agi::resources::fs::LoadImageRes(name, dir))
        return *fsResource;
    return LoadImageResInternal(name, dir);
}
/// Load icon resource using name.
inline wxIcon LoadIconRes(std::string_view name)
{
    if (boost::optional<wxIcon> fsResource = agi::resources::fs::LoadIconRes(name))
        return *fsResource;
    return LoadIconResInternal(name);
}
/// Load config string resource using name.
inline std::string LoadConfig(std::string_view name)
{
    if (boost::optional<std::string> fsResource = agi::resources::fs::LoadConfig(name))
        return *fsResource;
    return LoadConfigInternal(name);
}
}

#define GETIMAGE(name) agi::resources::LoadImageRes( #name )
#define GETICON(name) agi::resources::LoadIconRes( #name )
#define GETIMAGEDIR(name, dir) agi::resources::LoadImageRes( #name, dir )

#define GET_DEFAULT_CONFIG(name) agi::resources::LoadConfig( #name )
