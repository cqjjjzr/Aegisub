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

#include "include/aegisub/menu_dynamic.h"

#include <libaegisub/hotkey.h>
#include <libaegisub/id_allocator.h>

#include <algorithm>

#include "include/aegisub/hotkey.h"

wxString CommandDynamicMenuItem::GetDisplayName(agi::Context* context)
{
	if (command->Type() & cmd::COMMAND_DYNAMIC_NAME || overriddenText.IsEmpty())
		return command->StrMenu(context);
	return overriddenText;
}

wxString CommandDynamicMenuItem::GetHotkeyText(agi::Context*)
{
	return hotkey::get_hotkey_str_first("Default", command->name());
}

void DynamicMenuHelper::UpdateItem(std::shared_ptr<DynamicMenuItem> const& item, wxMenuItem* wxItem)
{
    wxItem->Enable(item->Validate(context));
	int flags = item->GetMenuItemType();
	UpdateItemName(item, wxItem);
	wxItem->SetHelp(item->GetHelp(context));
	if (flags & MENU_RADIO || flags & MENU_TOGGLE) {
		bool check = item->IsActive(context);
		// Don't call Check(false) on radio items as this causes wxGtk to
		// send a menu clicked event, and it should be a no-op anyway
		if (check || flags & MENU_TOGGLE)
			wxItem->Check(check);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
// This is NOT const.
void DynamicMenuHelper::UpdateItemName(std::shared_ptr<DynamicMenuItem> const& item, wxMenuItem* wxItem)
{
	wxItem->SetItemLabel(item->GetDisplayName(context) + '\t' + item->GetHotkeyText(context));
}

wxMenuItem* DynamicMenuHelper::CreateItem(std::shared_ptr<DynamicMenuItem> item, wxMenu* parent) const
{
	int flags = item->GetMenuItemType();
	wxItemKind kind =
		flags & MENU_RADIO ? wxITEM_RADIO :
		flags & MENU_TOGGLE ? wxITEM_CHECK :
		wxITEM_NORMAL;

	wxString menu_text = item->GetDisplayName(context) + '\t' + item->GetHotkeyText(context);

	wxMenuItem* menuItem = new wxMenuItem(parent, allocator->allocate(), menu_text, item->GetHelp(context), kind);
#ifndef __WXMAC__
	/// @todo Maybe make this a configuration option instead?
	if (kind == wxITEM_NORMAL)
		menuItem->SetBitmap(item->GetIcon(16));
#endif
	return menuItem;
}

DynamicMenuHelper::DynamicMenuHelper(agi::Context* context, agi::id_allocator* allocator)
{
	this->allocator = allocator;
    this->context = context;
    this->hotkeys_changed = hotkey::inst->AddHotkeyChangeListener(&DynamicMenuHelper::OnHotkeysChanged, this);
}

int DynamicMenuHelper::AddMenuItem(std::shared_ptr<DynamicMenuItem> item, wxMenu* parent)
{
	auto menuItem = CreateItem(item, parent);
	parent->Append(menuItem);
	auto id = menuItem->GetId();
	ForceRegisterId(id, item);
	wxMenuItems[id] = menuItem;
	return id;
}

int DynamicMenuHelper::PrependMenuItem(std::shared_ptr<DynamicMenuItem> item, wxMenu* parent)
{
	auto menuItem = CreateItem(item, parent);
	parent->Prepend(menuItem);
	auto id = menuItem->GetId();
	ForceRegisterId(id, item);
	wxMenuItems[id] = menuItem;
	return id;
}

std::shared_ptr<DynamicMenuItem> DynamicMenuHelper::FindItem(std::function<bool(DynamicMenuItem*)> pred)
{
	auto result = std::find_if(menuItems.begin(), menuItems.end(), [&](auto& ptr) -> bool { return pred(ptr.second.get()); });
	if (result != menuItems.end()) return std::shared_ptr(result->second);
	return std::shared_ptr<DynamicMenuItem>();
}

wxMenuItem* DynamicMenuHelper::FindWxItem(std::function<bool(DynamicMenuItem*)> pred)
{
	auto id = FindItemId(pred);
	if (id == -1) return nullptr;
	auto result2 = wxMenuItems.find(id);
	if (result2 == wxMenuItems.end()) return nullptr;
	return result2->second;
}

int DynamicMenuHelper::FindItemId(DynamicMenuItemPred pred)
{
	auto result = std::find_if(menuItems.begin(), menuItems.end(), [&](auto& ptr) -> bool { return pred(ptr.second.get()); });
	if (result == menuItems.end()) return -1;
	return result->first;
}

void DynamicMenuHelper::ForceRegisterId(const int id, std::shared_ptr<DynamicMenuItem> item)
{
	menuItems[id] = item;
}

void DynamicMenuHelper::Remove(wxMenuItem* item)
{
	Remove(item->GetId());
}

void DynamicMenuHelper::Remove(int id)
{
	menuItems.erase(id);
	wxMenuItems.erase(id);
	allocator->free(id);
}

void DynamicMenuHelper::Remove(DynamicMenuItemPred pred)
{
	auto result = std::find_if(menuItems.begin(), menuItems.end(), [&](auto& ptr) -> bool { return pred(ptr.second.get()); });
	if (result != menuItems.end()) Remove(result->first);
}

void DynamicMenuHelper::OnMenuOpen(wxMenuEvent& evt)
{
	for (auto& item : menuItems)
		if (item.second->IsDynamic())
		{
			auto wxItem = wxMenuItems.find(item.first);
			if (wxItem != wxMenuItems.end())
				UpdateItem(item.second, wxItem->second);
		}
}

void DynamicMenuHelper::OnMenuClick(wxCommandEvent& evt)
{
	auto func = menuItems.find(evt.GetId());
	if (func != menuItems.end()) func->second->OnClick(context);
}

void DynamicMenuHelper::OnHotkeysChanged()
{
	for (auto& item : menuItems)
	{
		auto wxItem = wxMenuItems.find(item.first);
		if (wxItem != wxMenuItems.end())
			UpdateItemName(item.second, wxItem->second);
	}
}

void DynamicMenuHelper::AttachMenu(wxEvtHandler* menu)
{
	menu->Bind(wxEVT_MENU_OPEN, &DynamicMenuHelper::OnMenuOpen, this);
	menu->Bind(wxEVT_MENU, &DynamicMenuHelper::OnMenuClick, this);
}
