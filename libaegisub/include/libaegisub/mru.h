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

#pragma once

#include <array>
#include <boost/filesystem/path.hpp>
#include <unordered_map>
#include <vector>
#include <functional>

#include <libaegisub/exception.h>
#include <libaegisub/fs_fwd.h>
#include <libaegisub/signal.h>

namespace json {
	class UnknownElement;
	typedef std::vector<UnknownElement> Array;
}

namespace agi {
struct Context;
class Options;

DEFINE_EXCEPTION(MRUError, Exception);

enum MruModificationType
{
    Add = 1,
	Remove = 2,
	MoveUp = 3
};

using MRUHandler = std::function<void(agi::Context*, agi::fs::path const&)>;
using MRUListMap = std::vector<agi::fs::path>;

class MRU
{
public:
	MRUListMap entries;
	MRUHandler handler;
	std::string config_name;
};

/// @class MRUManager
/// @brief Most Recently Used (MRU) list handling
///
/// Add() should be called anytime a file is opened, this will either add the
/// entry or update it if it already exists.
///
/// If a file fails to open, Remove() should be called.
class MRUManager {
public:
	/// @brief Constructor
	/// @param config File to load MRU values from
	MRUManager(agi::fs::path const& config, std::string default_config, agi::Options *options = nullptr);

	template<size_t N>
	MRUManager(agi::fs::path const& file, const char (&default_config)[N])
	: MRUManager(file, {default_config, N - 1}) { }

	/// @brief Add entry to the list.
	/// @param key List name
	/// @param entry Entry to add
	/// @exception MRUError thrown when an invalid key is used.
	void Add(const char *key, agi::fs::path const& entry);

	/// @brief Remove entry from the list.
	/// @param key List name
	/// @param entry Entry to add
	/// @exception MRUError thrown when an invalid key is used.
	void Remove(const char *key, agi::fs::path const& entry);

	/// @brief Return list
	/// @param key List name
	/// @exception MRUError thrown when an invalid key is used.
	const MRUListMap* Get(const char *key);

	MRU* GetMRU(std::string const& key);

	/// @brief Return A single entry in a list.
	/// @param key List name
	/// @param entry 0-base position of entry
	/// @exception MRUError thrown when an invalid key is used.
	agi::fs::path const& GetEntry(const char *key, const size_t entry);

	/// Write MRU lists to disk.
	void Flush();

private:
	agi::signal::Signal<MruModificationType, std::string const&, agi::fs::path const&> FireModifiedMru;
	/// Internal name of the config file, set during object construction.
	const agi::fs::path config_name;

	/// User preferences object for maximum number of items to list
	agi::Options *const options;

	/// Internal MRUMap values.
	std::unordered_map<std::string, MRU> mru;

	/// @brief Load MRU Lists.
	/// @param key List name.
	/// @param array json::Array of values.
	void Load(std::string const& key, ::json::Array const& array);
	/// @brief Prune MRUListMap to the desired length.
	/// This uses the user-set values for MRU list length.
	void Prune(std::string const& key, MRUListMap& map);
	MRUListMap &Find(const char *key);

public:
	DEFINE_SIGNAL_ADDERS(FireModifiedMru, AddMruModificationListener)
};

namespace mru
{
class MRUDescriptor
{
public:
	std::string name;
	MRUHandler handler;
	std::string config_name;
};
void RegisterMRU(MRUDescriptor const&);
}

} // namespace agi
