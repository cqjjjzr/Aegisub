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

#include "libaegisub/mru.h"

#include "libaegisub/cajun/writer.h"

#include "libaegisub/io.h"
#include "libaegisub/json.h"
#include "libaegisub/log.h"
#include "libaegisub/option.h"
#include "libaegisub/option_value.h"

#include <boost/range/adaptor/reversed.hpp>

namespace agi {
std::vector<mru::MRUDescriptor> mrus;
MRUManager::MRUManager(agi::fs::path const& config, std::string default_config, agi::Options *options)
: config_name(config)
, options(options)
{
	LOG_D("agi/mru") << "Loading MRU List";

	for (auto& desc : mrus)
	{
		mru[desc.name] = MRU{ MRUListMap(), desc.handler, desc.config_name };
	}
	auto root = json_util::file(config, default_config);
	for (auto const& it : static_cast<json::Object const&>(root))
		Load(it.first, it.second);
}

MRUListMap &MRUManager::Find(const char *key) {
	return GetMRU(key)->entries;
}

void mru::RegisterMRU(MRUDescriptor const& m)
{
	mrus.push_back(m);
}

void MRUManager::Add(const char *key, agi::fs::path const& entry) {
	MRUListMap &map = Find(key);
	auto it = find(begin(map), end(map), entry);
	if (it == begin(map) && it != end(map)) // Already the first entry
		return;
	if (it != end(map)) // Move the entry to the first
	{
		rotate(begin(map), it, it + 1);
		FireModifiedMru(MoveUp, std::string(key), entry);
	}
	else {
		map.insert(begin(map), entry);
		Prune(key, map);
		FireModifiedMru(MruModificationType::Add, std::string(key), entry);
	}

	Flush();
}

void MRUManager::Remove(const char *key, agi::fs::path const& entry) {
	auto& map = Find(key);
	map.erase(remove(begin(map), end(map), entry), end(map));
	FireModifiedMru(MruModificationType::Remove, std::string(key), entry);
	Flush();
}

const MRUListMap* MRUManager::Get(const char *key) {
	return &Find(key);
}

MRU* MRUManager::GetMRU(std::string const& key)
{
	auto ptr = mru.find(key);
	if (ptr == mru.end())
		throw MRUError("Invalid key value");
	return &ptr->second;
}

agi::fs::path const& MRUManager::GetEntry(const char *key, const size_t entry) {
	const auto map = Get(key);
	if (entry >= map->size())
		throw MRUError("Requested element index is out of range.");

	return *next(map->begin(), entry);
}

void MRUManager::Flush() {
	json::Object out;

	for (auto& [name, m] : mru)
	{
		json::Array& array = out[name];
		for (auto const& p : m.entries)
			array.push_back(p.string());
	}

	agi::JsonWriter::Write(out, io::Save(config_name).Get());
}

void MRUManager::Prune(std::string const& key, MRUListMap& map) {
	auto keyStr = std::string(key);
	size_t limit = 16u;
	if (options) {
		auto idx = mru.find(key);
		if (idx != mru.end())
			limit = static_cast<size_t>(options->Get(idx->second.config_name)->GetInt());
	}
	if (map.size() > limit)
		for (auto it = map.begin() + limit; it != map.end();)
		{
		    agi::fs::path path = *it;
			it = map.erase(it);
			FireModifiedMru(MruModificationType::Remove, keyStr, path);
		}
}

void MRUManager::Load(std::string const& key, const json::Array& array) {
	auto keyStr = std::string(key);
	auto ptr = mru.find(key);
	if (ptr == mru.end()) return;
	auto& entries = ptr->second.entries;

	try {
		entries.reserve(array.size());
		for (std::string const& str : boost::adaptors::reverse(array))
		{
			auto path = agi::fs::path(str);
			FireModifiedMru(MruModificationType::Add, keyStr, path);;
			entries.insert(entries.begin(), path);
		}
	}
	catch (json::Exception const&) {
		// Out of date MRU file; just discard the data and skip it
	}
	Prune(key, entries);
}

}
