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

#include "main_toolbar.h"

#include <wx/aui/aui.h>
#include "toolbar_utils.h"

namespace toolbar
{
json::Object const& get_root() {
	static json::Object root;
	if (root.empty()) {
		auto stream = std::stringstream(GET_DEFAULT_CONFIG(default_toolbar));
		root = std::move(static_cast<json::Object&>(agi::json_util::parse(stream)));
	}
	return root;
}

class MainToolbar : public wxAuiToolBar
{
private:
	/// Window ID of first toolbar control
	static const unsigned int TOOL_ID_BASE = 5000;
	/// Toolbar name in config file
	std::string name;
	/// Project context
	agi::Context* context;
	/// Commands for each of the buttons
	std::vector<cmd::Command*> commands;
	/// Hotkey context
	std::string hotkeyCtxt;

	RetinaHelper retina_helper;

	/// Current icon size
	int icon_size;

	/// Listener for icon size change signal
	agi::signal::Connection icon_size_slot;

	/// Listener for hotkey change signal
	agi::signal::Connection hotkeys_changed_slot;

	/// Enable/disable the toolbar buttons
	void OnIdle(wxIdleEvent&) {
		for (size_t i = 0; i < commands.size(); ++i) {
			if (commands[i]->Type() & cmd::COMMAND_VALIDATE)
				EnableTool(TOOL_ID_BASE + i, commands[i]->Validate(context));
			if (commands[i]->Type() & cmd::COMMAND_TOGGLE || commands[i]->Type() & cmd::COMMAND_RADIO)
				ToggleTool(TOOL_ID_BASE + i, commands[i]->IsActive(context));
		}
	}

	/// Toolbar button click handler
	void OnClick(wxCommandEvent& evt) {
		if (evt.GetId() > TOOL_ID_BASE)
		    (*commands[evt.GetId() - TOOL_ID_BASE])(context);
	}

	/// Regenerate the toolbar when the icon size changes
	void OnIconSizeChange(::agi::OptionValue const& opt) {
		icon_size = opt.GetInt();
		RegenerateToolbar();
	}

	/// Clear the toolbar and recreate it
	void RegenerateToolbar() {
		Unbind(wxEVT_IDLE, &MainToolbar::OnIdle, this);
		ClearTools();
		commands.clear();
		Populate(this, name, commands, context, icon_size, TOOL_ID_BASE, &MainToolbar::OnIdle);
	}
public:
	wxString GetTooltip(cmd::Command* command) const
	{
		wxString ret = command->StrHelp();

		std::vector<std::string> hotkeys = hotkey::get_hotkey_strs(hotkeyCtxt, command->name());
		if (!hotkeys.empty())
			ret += to_wx(" (" + boost::join(hotkeys, "/") + ")");

		return ret;
	}
	MainToolbar(wxFrame* parent, std::string name, agi::Context* c, std::string ht_context)
		: wxAuiToolBar(parent, -1, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW)
		, name(std::move(name))
		, context(c)
		, hotkeyCtxt(std::move(ht_context))
		, retina_helper(parent)
#ifndef __WXMAC__
		, icon_size(OPT_GET("App/Toolbar Icon Size")->GetInt())
		, icon_size_slot(OPT_SUB("App/Toolbar Icon Size", &MainToolbar::OnIconSizeChange, this))
#else
		, icon_size(32 * retina_helper.GetScaleFactor())
		, icon_size_slot(retina_helper.AddScaleFactorListener([=](double scale) {
		icon_size = 32 * retina_helper.GetScaleFactor();
		RegenerateToolbar();
	}))
#endif
		, hotkeys_changed_slot(hotkey::inst->AddHotkeyChangeListener(&MainToolbar::RegenerateToolbar, this))
	{
		Populate(this, this->name, commands, context, icon_size, TOOL_ID_BASE, &MainToolbar::OnIdle);
		if (!OPT_SET("App/Show Toolbar")->GetBool())
		    OPT_SET("App/Show Toolbar")->SetBool(true);
		Bind(wxEVT_TOOL, &MainToolbar::OnClick, this);
	}
};
void AttachMainToolbar(wxFrame* frame, wxAuiManager* manager, std::string const& name, agi::Context* context, std::string const& hotkey)
{
	auto& paneInfo = manager->GetPane("MainToolbar");
	if (&paneInfo == &wxAuiNullPaneInfo)
		manager->AddPane(new MainToolbar(frame, name, context, hotkey),
			wxAuiPaneInfo().Name("MainToolbar").Caption("Main Toolbar").ToolbarPane().Top());
	else
		paneInfo.Show(true);
	manager->Update();
}

void HideMainToolbar(wxAuiManager* manager)
{
	auto& paneInfo = manager->GetPane("MainToolbar");
	if (&paneInfo != &wxAuiNullPaneInfo)
		paneInfo.Show(false);
	manager->Update();
}
}
