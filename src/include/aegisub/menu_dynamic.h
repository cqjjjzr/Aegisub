// Copyright (c) 2020, Charlie Jiang <cqjjjzr@126.com>
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

#pragma once

#include "compat.h"

#include "command/command.h"
#include <wx/control.h>
#include <libaegisub/signal.h>

namespace agi
{
struct Context;
class id_allocator;
}

enum MenuItemType
{
	MENU_NORMAL = 0,

	/// Invoking this command toggles a setting of some sort. Any command
	/// of this type should have IsActive implemented to signal the
	/// current state of the thing being toggled, and invoking the command
	/// twice should be a no-op
	///
	/// This is mutually exclusive with MENU_RADIO
	MENU_TOGGLE = 1,

	/// Invoking this command sets a setting to a specific value. Any
	/// command of this type should have IsActive implemented, and if
	/// IsActive returns true, invoking the command should have no effect
	///
	/// This is mutually exclusive with MENU_TOGGLE
	MENU_RADIO = 2
};

class DynamicMenuItem
{
public:
    virtual ~DynamicMenuItem() = default;
    virtual wxString GetDisplayName(agi::Context*) = 0;
	virtual wxString GetHotkeyText(agi::Context*) = 0;
	virtual wxString GetHelp(agi::Context*) = 0;
	virtual wxBitmap GetIcon(int size, wxLayoutDirection dir = wxLayout_LeftToRight) { return wxBitmap{}; }
	virtual bool Validate(agi::Context*) = 0;
	virtual MenuItemType GetMenuItemType() = 0;
	virtual bool IsDynamic() = 0;
	virtual bool IsActive(agi::Context*) = 0;
	virtual void OnClick(agi::Context*) = 0;
};

class MinimalDynamicMenuItem : public DynamicMenuItem
{
	std::function<void(agi::Context*)> func;
public:
	MinimalDynamicMenuItem(std::function<void(agi::Context*)> func): func(func){}
	wxString GetDisplayName(agi::Context*) override { return ""; }
	wxString GetHotkeyText(agi::Context*) override { return ""; }
	wxString GetHelp(agi::Context*) override { return ""; }
	bool Validate(agi::Context*) override { return true; }
	MenuItemType GetMenuItemType() override { return MENU_NORMAL; }
	bool IsDynamic() override { return false; }
	bool IsActive(agi::Context*) override { return true; }
	void OnClick(agi::Context* context) override { func(context); }
};

class DefaultNormalDynamicMenuItem : public DynamicMenuItem
{
	wxString displayName, hotkeyText, help;
	wxBitmap icon;
	MenuItemType type;
	std::function<void(agi::Context*)> func;
public:
	DefaultNormalDynamicMenuItem(wxString displayName, wxString hotkeyText, wxString help, wxBitmap icon, MenuItemType type, std::function<void(agi::Context*)> func)
    : displayName(displayName), hotkeyText(hotkeyText), help(help), icon(icon), type(type), func(func) { }
	~DefaultNormalDynamicMenuItem() = default;
	wxString GetDisplayName(agi::Context*) override { return displayName; }
	wxString GetHotkeyText(agi::Context*) override { return hotkeyText; }
	wxString GetHelp(agi::Context*) override { return help; }
	wxBitmap GetIcon(int size, wxLayoutDirection dir = wxLayout_LeftToRight) override { return icon; }
	bool Validate(agi::Context*) override { return true; }
	MenuItemType GetMenuItemType() override { return MENU_NORMAL; }
	bool IsDynamic() override { return false; }
	bool IsActive(agi::Context*) override { return true; }
	void OnClick(agi::Context* context) override { func(context); }
};

class CommandDynamicMenuItem : public DynamicMenuItem
{
	cmd::Command* command;
	wxString overriddenText;
public:
	CommandDynamicMenuItem(cmd::Command* command, const wxString overriddenText)
    : command(command), overriddenText(overriddenText) {}
	CommandDynamicMenuItem(cmd::Command* command)
		: command(command), overriddenText("") {}
	~CommandDynamicMenuItem() = default;
	wxString GetDisplayName(agi::Context*) override;
	wxString GetHotkeyText(agi::Context*) override;
	wxString GetHelp(agi::Context*) override { return command->StrHelp(); }
	wxBitmap GetIcon(int size, wxLayoutDirection dir = wxLayout_LeftToRight) override { return command->Icon(size, dir); }
	bool Validate(agi::Context* context) override { return command->Validate(context); }
	MenuItemType GetMenuItemType() override
	{
		return command->Type() & cmd::COMMAND_RADIO ? MENU_RADIO
			: (command->Type() & cmd::COMMAND_TOGGLE ? MENU_TOGGLE : MENU_NORMAL);
	}
	bool IsDynamic() override { return command->Type() != cmd::COMMAND_NORMAL; }
	bool IsActive(agi::Context* context) override { return command->IsActive(context); }
	void OnClick(agi::Context* context) override { command->operator()(context); }
};

class DynamicMenuHelper {
	std::map<int, std::shared_ptr<DynamicMenuItem>> menuItems;
	std::map<int, wxMenuItem*> wxMenuItems;

	/// ID allocator for allocating menu item ids.
	agi::id_allocator* allocator;

	/// Project context
	agi::Context* context;

	/// Connection for hotkey change signal
	agi::signal::Connection hotkeys_changed;

	/// Update a single dynamic menu item
	void UpdateItem(std::shared_ptr<DynamicMenuItem> const& item, wxMenuItem* wxItem);
	void UpdateItemName(std::shared_ptr<DynamicMenuItem> const& item, wxMenuItem* wxItem);
    wxMenuItem* CreateItem(std::shared_ptr<DynamicMenuItem> item, wxMenu* parent) const;
public:
	using DynamicMenuItemPred = std::function<bool(DynamicMenuItem*)>;

	DynamicMenuHelper(agi::Context* context, agi::id_allocator* allocator);
	void SetContext(agi::Context* c) {
		context = c;
	}
	/// Append a command to a menu and register the needed handlers
	int AddMenuItem(std::shared_ptr<DynamicMenuItem> item, wxMenu* parent);
	/// Prepend a command to a menu and register the needed handlers
	int PrependMenuItem(std::shared_ptr<DynamicMenuItem> item, wxMenu* parent);

	std::shared_ptr<DynamicMenuItem> FindItem(std::function<bool(DynamicMenuItem*)> pred);
	wxMenuItem* FindWxItem(DynamicMenuItemPred pred);
	int FindItemId(DynamicMenuItemPred pred);

	/// Force register id with the given callback.
	///	USE WITH CARE.
	///	This should be only used for registering "wxWidgets standard ids" which can't be achieved from ID allocator.
	void ForceRegisterId(int id, std::shared_ptr<DynamicMenuItem> item);
	/// Unregister a dynamic menu item
	///	This frees the id of the item, so
	///	IMMEDIATELY remove the item from all menus after calling this function to avoid ID conflict.
	void Remove(wxMenuItem* item);
	/// Unregister a dynamic menu item
	///	This frees the id of the item, so
	///	*IMMEDIATELY* remove the item from all menus after calling this function to avoid ID conflict.
	void Remove(int id);
	void Remove(DynamicMenuItemPred pred);

	void OnMenuOpen(wxMenuEvent&);
	void OnMenuClick(wxCommandEvent& evt);
	/// Update the hotkeys for all menu items
	void OnHotkeysChanged();

	void AttachMenu(wxEvtHandler* menu);
};
