// Copyright (c) 2010, Amar Takhar <verm@aegisub.org>
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

#include "command.h"

#include "../compat.h"
#include "../format.h"
#include "../autoreghook.h"

#include <libaegisub/log.h>
#include <libaegisub/registry.h>

#include <wx/intl.h>
#include <utility>

namespace cmd {
    static agi::registry<Command> commands;
	typedef agi::registry<Command>::iterator iterator;

	static iterator find_command(std::string const& name) {
		auto it = commands.find_entry_iter(name);
		if (it == commands.end())
			throw CommandNotFound(agi::format(_("'%s' is not a valid command name"), name));
		return it;
	}


	void reg(std::unique_ptr<Command> cmd) {
		auto scmd = std::shared_ptr<Command>(std::move(cmd));
		commands.register_entry(scmd->name(), scmd);
	}

	void unreg(std::string const& name) {
		commands.unregister_entry(name);
	}

	Command *get(std::string const& name) {
		return find_command(name)->second.get();
	}

	void call(std::string const& name, agi::Context*c) {
		auto cmd = commands.find_entry(name);
		if (cmd && cmd->Validate(c))
			cmd->operator()(c);
	}

	std::vector<std::string> get_registered_commands() {
		std::vector<std::string> ret;
		for (auto const& it : std::as_const(commands))
			ret.push_back(it.first);
		return ret;
	}

	agi::registry<Command>& GetRegistry()
	{
		return commands;
	}


	// These forward declarations exist here since we don't want to expose
	// them in a header, they're strictly internal-use.
	void init_app();
	void init_audio();
	void init_automation();
	void init_command();
	void init_edit();
	void init_grid();
	void init_help();
	void init_keyframe();
	void init_recent();
	void init_subtitle();
	void init_time();
	void init_timecode();
	void init_tool();
	void init_video();
	void init_visual_tools();
	void init_builtin_commands() {
		LOG_D("command/init") << "Populating command map";
		init_app();
		init_audio();
		init_automation();
		init_edit();
		init_grid();
		init_help();
		init_keyframe();
		init_recent();
		init_subtitle();
		init_time();
		init_timecode();
		init_tool();
		init_video();
		init_visual_tools();
	}

	START_HOOK_FUNC(commands, init_builtin_commands)

	void clear() {
		for (auto& name : get_registered_commands())
			commands.unregister_entry(name);
	}
}
