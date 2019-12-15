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

#pragma once

#include <memory>
#include <unordered_map>
#include <iterator>

namespace agi {

template <typename T, typename list_type = std::unordered_map<std::string, std::shared_ptr<T>>>
class registry
{
private:
    list_type entries;
public:
    using iterator = typename list_type::const_iterator;

    typename list_type::const_iterator cbegin()
    {
        return entries.cbegin();
    }

    typename list_type::const_iterator cend()
    {
        return entries.cend();
    }

    typename list_type::const_iterator begin() const
    {
        return entries.cbegin();
    }

    typename list_type::const_iterator end() const
    {
        return entries.cend();
    }

    void register_entry(std::string_view name, std::shared_ptr<T> value)
    {
        auto strName = std::string(name);
        if (entries.find(strName) != entries.end())
            throw std::invalid_argument("Duplicate registration");

        entries[strName] = value;
    }

    void unregister_entry(std::string_view name)
    {
        entries.erase(std::string(name));
    }

    std::shared_ptr<T> find_entry(std::string_view name)
    {
        auto ptr = entries.find(std::string(name));
        if (ptr != entries.end()) return ptr->second;
        return std::shared_ptr<T>();
    }

    iterator find_entry_iter(std::string_view name)
    {
        return entries.find(std::string(name));
    }

    std::vector<std::string> get_entries_names()
    {
        auto ret = std::vector<std::string>();
        for (auto& it : entries)
            ret.push_back(it.first);
        return ret;
    }

    bool empty()
    {
        return entries.begin() == entries.end();
    }

    bool size()
    {
        return entries.size();
    }
};
}