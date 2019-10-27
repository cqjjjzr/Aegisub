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
//

#pragma once

#include <shared_mutex>
#include <type_traits>
#include <memory>

/// <summary>Cache to store two buffers. </summary>
/// This is a little bit similar to supporting <i>Producer-Consumer</i> model.
/// You can write and read at the same time on two threads.
/// The two buffers can improve performance since
/// when one thread is reading on one buffer, another thread can write to another buffer.
template<typename T, typename Delete = std::default_delete<T>>
class buffer_cache {
    static_assert(!std::is_rvalue_reference<Delete>::value,
                  "the specified deleter type cannot be an rvalue reference");
public:
    buffer_cache(T* buf1, T* buf2) : buf1(buf1), buf2(buf2) {}

    /// Get a buffer to write. Block until any buffer is available. <br>
    /// <b>WARNING: </b> Don't call this two times without releasing the firstly
    /// achieved buffer, or deadlock could happen.
    T* get_buffer_to_write() {
        if (mutex1.try_lock()) {
            latest = 1;
            return buf1;
        }
        latest = 2;
        mutex2.lock();
        return buf2;
    }

    /// Get the latest buffer to read. Block until the latest buffer is available. <br>
    /// <b>WARNING: </b> Don't call this two times without releasing the firstly
    /// achieved buffer, or deadlock could happen.
    T* get_latest_buffer_to_read() {
        if (latest == 1) {
            mutex1.lock_shared();
            return buf1;
        }
        mutex2.lock_shared();
        return buf2;
    }

    /// Release a buffer achieved through get_buffer_to_write()
    /// <b>WARNING:</b> Call this when current thread don't own the buffer causes UB.
    /// @see {get_buffer_to_write}
    void release_write(T* buf) {
        if (buf == buf1) mutex1.unlock();
        else if (buf == buf2) mutex2.unlock();
    }

    /// Release a buffer achieved through @ref get_latest_buffer_to_read() <br>
    /// <b>WARNING:</b> Call this when current thread don't own the buffer causes UB.
    /// @see {get_latest_buffer_to_read}
    void release_read(T* buf) {
        if (buf == buf1) mutex1.unlock_shared();
        else if (buf == buf2) mutex2.unlock_shared();
    }

    ~buffer_cache() {
        auto deleter = Delete();
        if (buf1) deleter(buf1);
        if (buf2) deleter(buf2);
    }
private:
    T *buf1, *buf2;
    std::shared_mutex mutex1, mutex2;
    int latest = 0;
};
