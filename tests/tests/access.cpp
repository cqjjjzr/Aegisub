// Copyright (c) 2010, Amar Takhar <verm@aegisub.org>
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

#include <main.h>

#include <libaegisub/access.h>
#include <libaegisub/fs.h>

using namespace agi;
using namespace agi::acs;
using namespace agi::fs;

// Yes, this is a horrifying massive code duplication.
// Since we are using GTest module of CMake we can't use macro to
// generate test cases since the gmake_add_tests wouldn't recognize them.

TEST(lagi_acs, CheckFileReadExFileNotFound) {
    EXPECT_THROW(CheckFileRead("data/nonexistent"), FileNotFound);
}
TEST(lagi_acs, CheckFileReadExReadDenied) {
    EXPECT_THROW(CheckFileRead("data/file_access_denied"), ReadDenied);
}
TEST(lagi_acs, CheckFileReadExNotAFile) {
    EXPECT_THROW(CheckFileRead("data/dir"), NotAFile);
}
TEST(lagi_acs, CheckFileRead) {
	EXPECT_NO_THROW(CheckFileRead("data/file"));
}

TEST(lagi_acs, CheckFileWriteExFileNotFound) {
    EXPECT_THROW(CheckFileWrite("data/nonexistent"), FileNotFound);
}
TEST(lagi_acs, CheckFileWriteExReadDenied) {
    EXPECT_THROW(CheckFileWrite("data/file_access_denied"), ReadDenied);
}
TEST(lagi_acs, CheckFileWriteExWriteDenied) {
    EXPECT_THROW(CheckFileWrite("data/file_read_only"), WriteDenied);
}
TEST(lagi_acs, CheckFileWriteExNotAFile) {
    EXPECT_THROW(CheckFileWrite("data/dir"), NotAFile);
}
TEST(lagi_acs, CheckFileWrite) {
    EXPECT_NO_THROW(CheckFileWrite("data/file"));
}

TEST(lagi_acs, CheckDirReadExFileNotFound) {
    EXPECT_THROW(CheckDirRead("data/nonexistent"), FileNotFound);
}
TEST(lagi_acs, CheckDirReadExReadDenied) {
    EXPECT_THROW(CheckDirRead("data/dir_access_denied"), ReadDenied);
}
TEST(lagi_acs, CheckDirReadExNotADirectory) {
    EXPECT_THROW(CheckDirRead("data/file"), NotADirectory);
}
TEST(lagi_acs, CheckDirRead) {
    EXPECT_NO_THROW(CheckDirRead("data/dir"));
}

TEST(lagi_acs, CheckDirWriteExFileNotFound) {
    EXPECT_THROW(CheckDirWrite("data/nonexistent"), FileNotFound);
}
TEST(lagi_acs, CheckDirWriteExReadDenied) {
    EXPECT_THROW(CheckDirWrite("data/dir_access_denied"), ReadDenied);
}
TEST(lagi_acs, CheckDirWriteExWriteDenied) {
    EXPECT_THROW(CheckDirWrite("data/dir_read_only"), WriteDenied);
}
TEST(lagi_acs, CheckDirWriteExNotADirectory) {
    EXPECT_THROW(CheckDirWrite("data/file"), NotADirectory);
}
TEST(lagi_acs, CheckDirWrite) {
    EXPECT_NO_THROW(CheckDirWrite("data/dir"));
}
