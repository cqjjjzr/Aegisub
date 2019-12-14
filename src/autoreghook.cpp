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

#include "autoreghook.h"

#include <list>

#include <libaegisub/exception.h>
#include <libaegisub/log.h>

namespace reg
{
static std::list<std::function<void()>>* funcs = nullptr;
static bool functionsCalled = false;

bool RegisterFunction(const std::function<void()> func)
{
    if (!funcs)
        funcs = new std::list<std::function<void()>>;
    funcs->push_back(func);
    return true;
}

void CallRegisteredFunctions()
{
    if (functionsCalled)
        throw agi::InternalError("Prohibited to call CallRegisteredFunctions() twice.");
    LOG_D("util/reg") << "Executing registered functions.";
    functionsCalled = true;

    for (auto& func : *funcs)
        func();
}
}
