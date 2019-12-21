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

/// @file toolbar_utils.h
/// @brief Utils to create toolbar.
/// @ingroup toolbar menu
///
#pragma once

#include <string>

#include <libaegisub/hotkey.h>
#include <libaegisub/json.h>
#include <libaegisub/log.h>
#include <libaegisub/signal.h>
#include <libaegisub/resources.h>
#include <libaegisub/exception.h>

#include <boost/algorithm/string/join.hpp>
#include <sstream>

#include "retina_helper.h"
#include "compat.h"
#include "include/aegisub/hotkey.h"
#include "options.h"
#include "command/command.h"

namespace toolbar
{
inline json::Object const& GetToolbarRoot() {
	static json::Object root;
	if (root.empty()) {
		auto stream = std::stringstream(GET_DEFAULT_CONFIG(default_toolbar));
		root = std::move(static_cast<json::Object&>(agi::json_util::parse(stream)));
	}
	return root;
}
/// Populate the toolbar with buttons
template <typename Toolbar, typename FuncOnIdle>
void Populate(Toolbar* tb, const std::string name, std::vector<cmd::Command*>& commands, agi::Context* context, int iconSize, int toolIdBase, FuncOnIdle OnIdle) {
	json::Object const& root = GetToolbarRoot();
	auto root_it = root.find(name);
	if (root_it == root.end()) {
		// Toolbar names are all hardcoded so this should never happen
		throw agi::InternalError("Toolbar named " + name + " not found.");
	}

	json::Array const& arr = root_it->second;
	commands.reserve(arr.size());
	bool hasDynamicTool = false;
	bool lastWasSeparator = false;

	// ReSharper disable once CppRangeBasedForIncompatibleReference
	for (json::String const& commandName : arr) {
		if (commandName.empty()) {
			if (!lastWasSeparator)
				tb->AddSeparator();
			lastWasSeparator = true;
			continue;
		}

		cmd::Command* command;
		try {
			command = cmd::get(commandName);
		}
		catch (cmd::CommandNotFound const&) {
			LOG_W("toolbar/command/not_found") << "Command '" << commandName << "' not found; skipping";
			continue;
		}

		lastWasSeparator = false;

		int flags = command->Type();
		wxItemKind kind =
			flags & cmd::COMMAND_RADIO ? wxITEM_RADIO :
			flags & cmd::COMMAND_TOGGLE ? wxITEM_CHECK :
			wxITEM_NORMAL;

		wxBitmap const& bitmap = command->Icon(iconSize, tb->GetLayoutDirection());
		tb->AddTool(toolIdBase + commands.size(), command->StrDisplay(context), bitmap, tb->GetTooltip(command), kind);

		commands.push_back(command);
		hasDynamicTool = hasDynamicTool || flags != cmd::COMMAND_NORMAL;
	}

	// Only bind the update function if there are actually any dynamic tools
	if (hasDynamicTool)
		tb->Bind(wxEVT_IDLE, OnIdle, tb);

	tb->Realize();
}
}
