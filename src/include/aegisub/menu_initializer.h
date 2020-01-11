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

/// @file menu_initializer.h
/// @brief Menu initializer tool.
/// @ingroup menu

#pragma once

#include <boost/any.hpp>
#include <utility>
#include <vector>
#include <initializer_list>
#include <type_traits>

#include "menu.h"

namespace menu {
enum MenuNodeType
{
    command = 0,
    submenu = 1,
    separator = 2,
    dynamic = 3
};

enum SpecialMenuNode
{
    none = -1,
    help = 0,
    options = 1,
    about = 2,
    exit = 3,
    window = 4
};

DEFINE_EXCEPTION(BadMenuTypeException, agi::Exception);

class MenuNode
{
public:
    MenuNodeType type;
    boost::any value;
    SpecialMenuNode special = none;
    std::string text = "";
    UpdateableMenu* dynamicMenu = nullptr;

    MenuNode(): type(separator) { }
    MenuNode(std::string const& commandName): type(command), value(commandName) {}
    MenuNode(std::string const& commandName, SpecialMenuNode const& special): type(command), value(commandName), special(special) {}
    MenuNode(std::string const& commandName, std::string commandOverrideText, SpecialMenuNode const& special = none)
    : type(command), value(commandName), special(special), text(std::move(commandOverrideText)) {}
    MenuNode(std::string submenuName, std::initializer_list<MenuNode> submenuNodes) : type(submenu),
        value(std::vector<MenuNode>(submenuNodes)), text(std::move(submenuName)) {}
    MenuNode(std::string submenuName, std::initializer_list<MenuNode> submenuNodes, SpecialMenuNode const& special) : type(submenu),
        value(std::vector<MenuNode>(submenuNodes)), special(special), text(std::move(submenuName)){}

    MenuNode(std::string submenuName, UpdateableMenu* dynamicMenu): type(dynamic), dynamicMenu(dynamicMenu), text(std::move(submenuName)) {}

    std::string GetName() const {
        if (type == submenu || type == dynamic) return text;
        if (type == command) return boost::any_cast<std::string>(value);
        return "";
    }

    std::string GetText() const
    {
        return text;
    }

    wxMenu* GetSubmenu() {
        return dynamicMenu;
    }

    std::vector<MenuNode>& GetSubmenuNodes()
    {
        if (type == submenu)
            return boost::any_cast<std::vector<MenuNode>&>(value);
        throw BadMenuTypeException("Internal error: Bad submenu type!");
    }

    void UpdateDynamicMenu()
    {
        if (type == dynamic)
            boost::any_cast<UpdateableMenu*>(value)->UpdateMenu();
    }
};

}
