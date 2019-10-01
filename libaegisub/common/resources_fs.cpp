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
//

#include "libaegisub/resources_fs.h"

#include "libaegisub/io.h"
#include "libaegisub/fs.h"
#include "libaegisub/make_unique.h"
#include "libaegisub/util.h"

#include <wx/intl.h>
#include <wx/icon.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/mstream.h>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>
#include <boost/none.hpp>

namespace agi::resources::fs
{
const std::string imageResourceExt = ".png";
const std::string iconResourceExt = ".png";
const std::string configResourceExt = ".json";
const std::string resourcePathPrefix = "resources/";

wxImage LoadWxImage(std::string& name)
{
    auto ptrStream = agi::io::Open(name, true);
    auto& stream = *ptrStream;
    auto len = agi::fs::Size(name);
    auto buf = std::unique_ptr<char>(new char[len]);
    if (!(ptrStream->read(buf.get(), len)))
        throw agi::io::IOFatal("Unknown fatal error occurred reading " + name);

    wxMemoryInputStream mem(buf.get(), len);
    return wxImage(mem);
}

boost::optional<wxBitmap> LoadImageRes(std::string_view name, int dir)
{
    auto filename = resourcePathPrefix + std::string(name) + imageResourceExt;
    if (!agi::fs::FileExists(filename)) return boost::none;
    auto image = LoadWxImage(filename);
    if (dir != wxLayout_RightToLeft)
        return wxBitmap(image);
    return wxBitmap(image.Mirror());
}
boost::optional<wxIcon> LoadIconRes(std::string_view name)
{
    auto filename = resourcePathPrefix + std::string(name) + iconResourceExt;
    if (!agi::fs::FileExists(filename)) return boost::none;
    auto image = LoadWxImage(filename);
    wxIcon icon;
    icon.CopyFromBitmap(wxBitmap(image));
    return icon;
}
boost::optional<std::string> LoadConfig(std::string_view name)
{
    auto filename = resourcePathPrefix + std::string(name) + configResourceExt;
    if (!agi::fs::FileExists(filename)) return boost::none;
    auto len = agi::fs::Size(filename);
    return agi::io::ReadFile(filename);
}
}
