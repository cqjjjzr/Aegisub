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

/// @file menu_dynamic_builtin.h
/// @brief Builtin dynamic menus.
/// @ingroup menu

#pragma once

#include "include/aegisub/menu.h"
#include <libaegisub/id_allocator.h>
#include <libaegisub/mru.h>

namespace menu
{
extern agi::id_allocator* builtinAllocator;

class MruMenu : public UpdateableMenu
{
	std::string type;
	agi::MRU* mru;
	DynamicMenuHelper helper;
	agi::Context* c;

	agi::signal::Connection mruModificationConn;
	void OnMruModification(agi::MruModificationType changeType, std::string const& key, agi::fs::path const& entry);
public:
	MruMenu(std::string const& type, agi::Context* ctx);
	void UpdateMenu() override;
};

class AutomationMenu : public UpdateableMenu
{
	agi::Context* c;
	agi::signal::Connection global_slot;
	agi::signal::Connection local_slot;
	std::vector<wxMenuItem*> all_items;
public:
	void RegenerateMenu();
	void UpdateMenu() {}

	AutomationMenu(agi::Context* ctxt);
	~AutomationMenu();
};
}
