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

#include "menu_dynamic_builtin.h"

#include <libaegisub/mru.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/locale/collator.hpp>

#include "format.h"
#include "options.h"
#include "compat.h"

namespace menu
{
agi::id_allocator* builtinAllocator = new agi::id_allocator(10000, 50000);

class MruMenuItem : public DynamicMenuItem
{
	friend class MruMenu;
	agi::MRU* mru;
	agi::fs::path path;
public:
	MruMenuItem(agi::MRU* mru, agi::fs::path path): mru(mru), path(path) {}
    ~MruMenuItem() override {}
	wxString GetDisplayName(agi::Context*) override { return to_wx(path.string()); }
	wxString GetHotkeyText(agi::Context*) override { return ""; }
	wxString GetHelp(agi::Context*) override { return ""; }
	bool Validate(agi::Context*) override { return true; }
	MenuItemType GetMenuItemType() override { return MENU_NORMAL; }
	bool IsDynamic() override { return false; }
	bool IsActive(agi::Context*) override { return false; }
    void OnClick(agi::Context* ctx) override
	{
		mru->handler(ctx, path);
	}
};

void MruMenu::OnMruModification(agi::MruModificationType changeType, std::string const& key, agi::fs::path const& entry)
{
	if (key != type) return;
	switch (changeType)
	{
	case agi::MruModificationType::Add:
		helper.PrependMenuItem(std::make_shared<MruMenuItem>(mru, entry), this);
		break;
	case agi::MruModificationType::Remove:
	    {
		auto wxmi = helper.FindWxItem([&](DynamicMenuItem* p) -> bool
		{
			auto mi = dynamic_cast<MruMenuItem*>(p);
			if (mi == nullptr) return false;
			return mi->mru == this->mru && mi->path == entry;
		});
		helper.Remove(wxmi);
		Delete(wxmi);
	    }
		break;
	case agi::MruModificationType::MoveUp:
	    {
		auto wxmi = helper.FindWxItem([&](DynamicMenuItem* p) -> bool
		{
			auto mi = dynamic_cast<MruMenuItem*>(p);
			if (mi == nullptr) return false;
			return mi->mru == this->mru && mi->path == entry;
		});
		if (wxmi == nullptr) return;
		Remove(wxmi);
		Prepend(wxmi);
	    }
		break;
	}
}

MruMenu::MruMenu(std::string const& type, agi::Context* ctx)
    : type(type)
	, mru(config::mru->GetMRU(this->type))
    , helper(ctx, builtinAllocator)
    , c(ctx)
    , mruModificationConn(config::mru->AddMruModificationListener(&MruMenu::OnMruModification, this))
{
	helper.AttachMenu(this);
	for (auto entry : mru->entries)
		helper.AddMenuItem(std::make_shared<MruMenuItem>(mru, entry), this);
}

void MruMenu::UpdateMenu()
{

}

struct WorkItem {
	std::string displayname;
	cmd::Command* command;
	std::vector<WorkItem> subitems;

	WorkItem(std::string const& displayname, cmd::Command* command = nullptr)
		: displayname(displayname), command(command) { }

	WorkItem* FindOrMakeSubitem(std::string const& name) {
		auto sub = std::find_if(subitems.begin(), subitems.end(), [&](WorkItem const& item) { return item.displayname == name; });
		if (sub != subitems.end()) return &*sub;

		subitems.emplace_back(name);
		return &subitems.back();
	}

	void Sort() {
		if (command) return;
		for (auto& sub : subitems)
			sub.Sort();
		auto comp = boost::locale::comparator<std::string::value_type>();
		std::sort(subitems.begin(), subitems.end(), [&](WorkItem const& a, WorkItem const& b) {
			return comp(a.displayname, b.displayname);
		});
	}

	void GenerateMenu(wxMenu* parent, AutomationMenu* am) {
		for (auto item : subitems) {
			if (item.command) {
				//am->cm->AddCommand(item.command, parent, item.displayname);
				//am->all_items.push_back(parent->GetMenuItems().back());
			}
			else {
				auto submenu = new wxMenu;
				parent->AppendSubMenu(submenu, to_wx(item.displayname));
				item.GenerateMenu(submenu, am);
			}
		}
	}
};

}
