// Copyright (c) 2020, Charlie Jiang <cqjjjzr@126.com>
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

#include <boost/numeric/interval.hpp>
#include <set>
#include <limits>

namespace agi {

class id_interval
{
public:
    id_interval() : id_interval(0, 0) {}
    id_interval(const int ll, const int uu) : value(ll, uu) {}
    bool operator < (const id_interval& another) const { return value.upper() < another.value.lower(); }
    [[nodiscard]] int left() const { return value.lower(); }
    [[nodiscard]] int right() const { return value.upper(); }

    [[nodiscard]] bool is_in(int id) const { return value.lower() >= id && value.upper() <= id; }
private:
    boost::numeric::interval<int> value;
};

class id_allocator {
public:
    id_allocator() : id_allocator(0, std::numeric_limits<int>::max()) {}
    id_allocator(int minId, int maxId);
    int allocate();          // Allocates an id
    void free(int id);       // Frees an id so it can be used again

    bool is_in(int id) const { return limit.is_in(id); }
private:
    using id_intervals_t = std::set<id_interval>;
    id_intervals_t free_intervals;

    id_interval limit;
};

}
