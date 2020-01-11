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

/// @file menu.cpp
/// @brief Dynamic menu and toolbar generator.
/// @ingroup menu

#include "include/aegisub/menu.h"

#include "include/aegisub/context.h"
#include "include/aegisub/hotkey.h"
#include "include/aegisub/menu_initializer.h"
#include "include/aegisub/menu_dynamic.h"

#include "auto4_base.h"
#include "command/command.h"
#include "compat.h"
#include "format.h"
#include "libaegisub/resources.h"
#include "menu_dynamic_builtin.h"

#include <libaegisub/log.h>
#include <libaegisub/make_unique.h>

#include <boost/range/algorithm_ext/push_back.hpp>
#include <vector>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/menuitem.h>

#ifdef __WXMAC__
#include <wx/app.h>
#endif

namespace {
void processMenuItem(menu::MenuNode& item, wxMenu* meparentnu, agi::Context* context, DynamicMenuHelper* cm);
wxMenu* buildMenu(std::vector<menu::MenuNode>& items, agi::Context* c, DynamicMenuHelper* cm, wxMenu* menu = nullptr);

wxMenu* buildMenu(std::vector<menu::MenuNode>& items, agi::Context* c, DynamicMenuHelper* cm, wxMenu* menu)
{
	if (!menu) menu = new wxMenu;
	for (auto& item : items)
		processMenuItem(item, menu, c, cm);
	return menu;
}

void processMenuItem(menu::MenuNode& item, wxMenu* parent, agi::Context* context, DynamicMenuHelper* cm)
{
	auto name = to_wx(item.GetText());
	try
	{
		switch (item.type)
		{
		case menu::MenuNodeType::separator:
			parent->AppendSeparator();
			return;
		case menu::MenuNodeType::submenu:
			parent->AppendSubMenu(buildMenu(item.GetSubmenuNodes(), context, cm), name);
#ifdef __WXMAC__
			if (item.special == menu::help)
				wxApp::s_macHelpMenuTitleName = name;
#endif
			return;
		case menu::MenuNodeType::command:
		{
			int id = cm->AddMenuItem(std::make_shared<CommandDynamicMenuItem>(cmd::get(item.GetName()), to_wx(item.GetText())), parent);
#ifdef __WXMAC__
			switch (item.special)
			{
			case menu::about:
				wxApp::s_macAboutMenuItemId = id; break;
			case menu::exit:
				wxApp::s_macExitMenuItemId = id; break;
			case menu::options:
				wxApp::s_macPreferencesMenuItemId = id; break;
			default: break;
			}
#else
			(void)id; // Disable IDE warning for unused variable.
#endif
			return;
		}
		case menu::MenuNodeType::dynamic:
			parent->AppendSubMenu(item.GetSubmenu(), name);
			return;
		default:
#ifdef _DEBUG
			parent->Append(-1, to_wx("Invalid dynamic menu!"))->Enable(false);
#endif
		}
	} catch (agi::Exception const& e) {
#ifdef _DEBUG
		parent->Append(-1, to_wx(e.GetMessage()))->Enable(false);
#endif
		LOG_W("menu/command/not_found") << "Skipping command: " << e.GetMessage();
	}
}

/*class AutomationMenu final : public wxMenu {
	agi::Context *c;
	CommandManager *cm;
	agi::signal::Connection global_slot;
	agi::signal::Connection local_slot;
	std::vector<wxMenuItem *> all_items;

	struct WorkItem {
		std::string displayname;
		cmd::Command *command;
		std::vector<WorkItem> subitems;

		WorkItem(std::string const &displayname, cmd::Command *command = nullptr)
		: displayname(displayname), command(command) { }

		WorkItem *FindOrMakeSubitem(std::string const &name) {
			auto sub = std::find_if(subitems.begin(), subitems.end(), [&](WorkItem const &item) { return item.displayname == name; });
			if (sub != subitems.end()) return &*sub;

			subitems.emplace_back(name);
			return &subitems.back();
		}

		void Sort() {
			if (command) return;
			for (auto &sub : subitems)
				sub.Sort();
			auto comp = boost::locale::comparator<std::string::value_type>();
			std::sort(subitems.begin(), subitems.end(), [&](WorkItem const &a, WorkItem const &b){
				return comp(a.displayname, b.displayname);
			});
		}

		void GenerateMenu(wxMenu *parent, AutomationMenu *am) {
			for (auto item : subitems) {
				if (item.command) {
					am->cm->AddCommand(item.command, parent, item.displayname);
					am->all_items.push_back(parent->GetMenuItems().back());
				}
				else {
					auto submenu = new wxMenu;
					parent->AppendSubMenu(submenu, to_wx(item.displayname));
					item.GenerateMenu(submenu, am);
				}
			}
		}
	};

	void Regenerate() {
		for (auto item : all_items)
			cm->Remove(item);

		wxMenuItemList &items = GetMenuItems();
		// Remove everything but automation manager and the separator
		for (size_t i = items.size() - 1; i >= 2; --i)
			Delete(items[i]);

		auto macros = config::global_scripts->GetMacros();
		boost::push_back(macros, c->local_scripts->GetMacros());
		if (macros.empty()) {
			Append(-1, _("No Automation macros loaded"))->Enable(false);
			return;
		}

		WorkItem top("");
		for (auto macro : macros) {
			const auto name = from_wx(macro->StrMenu(c));
			WorkItem *parent = &top;
			for (auto section : agi::Split(name, wxS('/'))) {
				std::string sectionname(section.begin(), section.end());

				if (section.end() == name.end()) {
					parent->subitems.emplace_back(sectionname, macro);
				}
				else {
					parent = parent->FindOrMakeSubitem(sectionname);
				}
			}
		}
		top.Sort();
		top.GenerateMenu(this, this);
	}
public:
	AutomationMenu(agi::Context *c, CommandManager *cm)
	: c(c)
	, cm(cm)
	, global_slot(config::global_scripts->AddScriptChangeListener(&AutomationMenu::Regenerate, this))
	, local_slot(c->local_scripts->AddScriptChangeListener(&AutomationMenu::Regenerate, this))
	{
		cm->AddCommand(cmd::get("am/meta"), this);
		AppendSeparator();
		Regenerate();
	}
};*/
}

namespace menu {
	void AttachMenuBar(MenuProvider provider, wxFrame *window, agi::Context *c, agi::id_allocator* allocator) {
#ifdef __WXMAC__
		auto bind_events = [&](CommandMenuBar *menu) {
			window->Bind(wxEVT_ACTIVATE, [=](wxActivateEvent&) { menu->cm.SetContext(c); });
			window->Bind(wxEVT_DESTROY, [=](wxWindowDestroyEvent&) {
				if (!osx::activate_top_window_other_than(window))
					menu->cm.SetContext(nullptr);
			});
		};

		if (wxMenuBar *menu = wxMenuBar::MacGetCommonMenuBar()) {
			bind_events(static_cast<CommandMenuBar *>(menu));
			return;
		}
#endif

		auto menuNode = provider(c);

		auto menu = agi::make_unique<CommandMenuBar>(c, allocator);
		for (auto& item : menuNode)
		{
#ifdef __WXMAC__
			if (item.special == menu::window)
				osx::make_windows_menu(parent);
#endif
			if (item.type == submenu)
				menu->Append(buildMenu(item.GetSubmenuNodes(), c, &menu->cm), _(to_wx(item.GetName())));
			else if (item.type == dynamic)
				menu->Append(item.GetSubmenu(), _(to_wx(item.GetName())));
		}

		menu->cm.AttachMenu(menu.get());

#ifdef __WXMAC__
		bind_events(menu.get());
		wxMenuBar::MacSetCommonMenuBar(menu.get());
#else
		window->SetMenuBar(menu.get());
#endif

		menu.release();
	}

	std::unique_ptr<wxMenu> GetMenu(MenuProvider provider, agi::Context *c, agi::id_allocator* allocator) {
		auto menu = agi::make_unique<CommandMenu>(c, allocator);
		auto items = provider(c);
		buildMenu(items, c, &menu->cm, menu.get());
		menu->cm.AttachMenu(menu.get());
		return std::unique_ptr<wxMenu>(menu.release());
	}

	void OpenPopupMenu(wxMenu *menu, wxWindow *parent_window) {
		wxMenuEvent evt;
		evt.SetEventType(wxEVT_MENU_OPEN);
		menu->ProcessEvent(evt);
		parent_window->PopupMenu(menu);
	}
}
