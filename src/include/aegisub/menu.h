// Copyright (c) 2011, Thomas Goyne <plorkyeran@aegisub.org>
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

/// @file menu.h
/// @brief Dynamic menu and toolbar generator.
/// @ingroup menu toolbar
#pragma once

#include <memory>
#include <string>

#include <libaegisub/exception.h>
#include "libaegisub/signal.h"
#include "menu_dynamic.h"

namespace cmd {
    class Command;
}

namespace agi
{
    struct Context;
	class id_allocator;
}

class wxFrame;
class wxMenu;
class wxMenuBar;
class wxWindow;

namespace menu {
	DEFINE_EXCEPTION(Error, agi::Exception);
	DEFINE_EXCEPTION(UnknownMenu, Error);
	DEFINE_EXCEPTION(InvalidMenu, Error);

	class UpdateableMenu : public wxMenu
	{
	public:
		virtual void UpdateMenu() = 0;
	};
	/// Wrapper for wxMenu to add a command manager
	struct CommandMenu final : public wxMenu {
		DynamicMenuHelper cm;
		CommandMenu(agi::Context* c, agi::id_allocator* allocator) : cm(c, allocator) {}
	};

	/// Wrapper for wxMenuBar to add a command manager
	struct CommandMenuBar final : public wxMenuBar {
		DynamicMenuHelper cm;
		CommandMenuBar(agi::Context* c, agi::id_allocator* allocator) : cm(c, allocator) { }
	};

	class MenuNode;
	using MenuProvider = std::function<std::vector<MenuNode>(agi::Context*)>;

	/// @brief Get the menu with the specified name as a wxMenuBar
	/// @param name Name of the menu
	///
	/// Throws:
	///     UnknownMenu if no menu with the given name was found
	///     BadMenu if there is a menu with the given name, but it is invalid
	void AttachMenuBar(MenuProvider provider, wxFrame *window, agi::Context *c, agi::id_allocator* allocator);

	/// @brief Get the menu with the specified name as a wxMenu
	/// @param name Name of the menu
	///
	/// Throws:
	///     UnknownMenu if no menu with the given name was found
	///     BadMenu if there is a menu with the given name, but it is invalid
	std::unique_ptr<wxMenu> GetMenu(MenuProvider provider, agi::Context *c, agi::id_allocator* allocator);

	/// @brief Open a popup menu at the mouse
	/// @param menu Menu to open
	/// @param parent_window Parent window for the menu; cannot be NULL
	///
	/// This function should be used rather than wxWindow::PopupMenu due to
	/// that PopupMenu does not trigger menu open events and triggers update
	/// ui events on the opening window rather than the menu for some bizarre
	/// reason
	void OpenPopupMenu(wxMenu *menu, wxWindow *parent_window);
}
