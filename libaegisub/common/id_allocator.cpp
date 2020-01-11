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

#include "libaegisub/id_allocator.h"

namespace agi {
id_allocator::id_allocator(int minId, int maxId)
{
    free_intervals.emplace(minId, maxId);
    limit = id_interval(minId, maxId);
}

int id_allocator::allocate()
{
    id_interval first = *(free_intervals.begin());
    int free_id = first.left();
    free_intervals.erase(free_intervals.begin());
    if (first.left() + 1 <= first.right())
        free_intervals.emplace(first.left() + 1, first.right());
    return free_id;
}

void id_allocator::free(int id)
{
    auto after = free_intervals.upper_bound(id_interval(id, id));
    auto before = after;
    auto has_after = after != free_intervals.end();
    auto has_before = before != free_intervals.begin();
    if (has_before) {
        --before; // Move before to the real `before`
        if (before->is_in(id)) return; // Already free!
    }

    auto after_linkable = has_after && after->left() == id + 1;
    auto before_linkable = has_before && before->right() == id - 1;

    if (before_linkable && after_linkable)
    {
        int l = before->left(), r = after->right();
        auto temp = after;
        if (has_after) ++temp;
        free_intervals.erase(after);
        free_intervals.erase(before);
        if (temp != free_intervals.end())
            free_intervals.emplace_hint(temp, l, r);
        else
            free_intervals.emplace(l, r);
        return;
    }
    if (before_linkable)
    {
        int l = before->left(), r = id;
        free_intervals.erase(before);
        if (has_after)
            free_intervals.emplace_hint(after, l, r);
        else
            free_intervals.emplace(l, r);
        return;
    }
    if (after_linkable)
    {
        int l = id, r = after->right();
        free_intervals.erase(after);
        if (has_before)
            free_intervals.emplace_hint(before, l, r);
        else
            free_intervals.emplace(l, r);
        return;
    }

    free_intervals.emplace(id, id);
}
}
