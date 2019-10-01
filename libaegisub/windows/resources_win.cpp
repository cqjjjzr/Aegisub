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

#include "libaegisub/resources.h"
#include "libaegisub/charset_conv_win.h"
#include "libaegisub/exception.h"
#include "libaegisub/util.h"

#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/mstream.h>

#include <windows.h>

namespace agi::resources
{

DEFINE_EXCEPTION(BadResourceException, Exception);

std::unique_ptr<char> LoadBinary(const std::string& name, OUT size_t& outLength)
{
	auto nameStr = charset::ConvertW(name);
	HRSRC resSrc = FindResource(NULL, nameStr.c_str(), RT_RCDATA);
	if (resSrc == NULL)
		throw BadResourceException("Failed to find resource " + name + ":" + util::ErrorString(GetLastError()));
	HGLOBAL res = LoadResource(NULL, resSrc);
	if (res == NULL)
		throw BadResourceException("Failed to load resource " + name + ":" + util::ErrorString(GetLastError()));

	DWORD size = SizeofResource(NULL, resSrc);
	if (size <= 0)
		throw BadResourceException("Failed to load resource " + name + ", the reported size was 0. " + util::ErrorString(GetLastError()));
	auto buf = std::unique_ptr<char>(new char[size]);
	LPVOID originBuf = LockResource(res);
	if (originBuf == NULL)
		throw BadResourceException("Failed to load resource " + name + ", failed to get resource buffer. " + util::ErrorString(GetLastError()));
	CopyMemory(buf.get(), originBuf, size);
	outLength = size;

	return buf;
}

wxBitmap LoadImageResInternal(std::string_view name, int dir)
{
	size_t len;
	auto buf = LoadBinary("bitmap_" + std::string(name), len);

	wxMemoryInputStream mem(buf.get(), len);
	if (dir != wxLayout_RightToLeft)
		return wxBitmap(wxImage(mem));
	return wxBitmap(wxImage(mem).Mirror());
}

wxIcon LoadIconResInternal(std::string_view name)
{
	size_t len;
	auto buf = LoadBinary("bitmap_" + std::string(name), len);

	wxMemoryInputStream mem(buf.get(), len);
	wxIcon icon;
	icon.CopyFromBitmap(wxBitmap(wxImage(mem)));
	return icon;
}

std::string LoadConfigInternal(std::string_view name)
{
	size_t len;
	auto buf = LoadBinary("config_" + std::string(name), len);

	return std::string(buf.get(), len);
}
}
