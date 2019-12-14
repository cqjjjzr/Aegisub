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
/// @file autoreghook.h
/// @brief Util for built-in components to register automatically-executed hooks(e.g. to register components).
/// @ingroup util
///

#pragma once

/// Util for built-in components to register automatically-executed hooks(e.g. to register components).
namespace reg
{
/// Register a function to be called in the future.
/// This should be only called in the initialization of static variables.
/// @code
/// static bool componentAInitialized = reg::RegisterFunction([]()->{
///     registry.registerEntry(......);
/// });
/// @endcode
bool RegisterFunction(std::function<void()>);
/// INTERNAL USE ONLY
/// Call all registered functions.
/// This function can be called ONLY ONCE, and should be called at the starting stage.
/// @throws agi::InternalException When called the second time.
void CallRegisteredFunctions();
}

#define START_HOOK_BEGIN(name) bool name##Registered = reg::RegisterFunction([]()->void{
#define START_HOOK_END });
#define START_HOOK_FUNC(name, func) bool name##Registered = reg::RegisterFunction(func);
