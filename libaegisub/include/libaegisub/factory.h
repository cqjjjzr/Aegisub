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

#include <functional>
#include <list>
#include <string>
#include <memory>

#include <boost/algorithm/string.hpp>
#include <csignal>

namespace agi
{
template <class T>
using DefaultFactoryFunc = std::function<std::unique_ptr<T>()>;

template <typename T, typename Func = DefaultFactoryFunc<T>>
struct factory
{
	std::string name;
	Func create;
	bool hidden;

	factory(std::string& name, Func create, bool hidden)
		: name(name), create(create), hidden(hidden)
	{ }
};

template <typename T, typename Func = DefaultFactoryFunc<T>, typename Iter>
std::list<T>& SortedFactories(Iter begin, Iter end, const char* preferred)
{
	auto hidden = std::list<T>();
	auto ret = std::list<T>();
	for (Iter it = begin; it != end; ++it)
		if (it->hidden)
			hidden.push_back(*it);
		else if (strcmp(preferred, it->name) == 0)
			ret.push_front(*it);
		else
			ret.push_back(*it);
	ret.splice(ret.begin(), hidden);

	return ret;
}

struct factory_comparator
{
	std::string preferred;

	template<typename T, typename Func = DefaultFactoryFunc<T>>
	bool operator()(const factory<T, Func>* t, const factory<T, Func>* u) const {
		if (t == u) return false;
		if (t->hidden && !u->hidden) return true;
		if (!t->hidden && u->hidden) return false;

		if (boost::iequals(preferred, t->name)) return true;
	    if (boost::iequals(preferred, u->name)) return false;

		return t->name < u->name;
	}
};
}
