// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "timeout.hpp"
#include <splat/warnings.h>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <mutex>

namespace pex {

PRAGMA_WARNING_PUSH
DISABLE_MSVC_WARNING(4324) // structure was padded due to alignment specifier

class atomic_cvar {
    // align to prevent false sharing
    // can't use std::hardware_destructive_interference_size as this is a part of public ABI
#if !defined(_MSC_VER)
    // temporarily disable this for msvc until tehre's a resolution on
    // https://developercommunity.microsoft.com/t/Address-sanitizer-bug-when-deducing-temp/10852062
    alignas(64)
#endif
        std::atomic_bool m_flag;

    std::mutex m_mutex;
    std::condition_variable m_cvar;

    bool wait_pred() {
        return m_flag.exchange(false, std::memory_order_acquire);
    }
public:
    void notify_one() {
        m_flag.store(true, std::memory_order_release);
        m_cvar.notify_one();
    }

    bool wait() {
        std::unique_lock lock(m_mutex);
        m_cvar.wait(lock, [&] { return wait_pred(); });
        return true;
    }

    bool wait_for(std::chrono::milliseconds d) {
        std::unique_lock lock(m_mutex);
        return m_cvar.wait_for(lock, d, [&] { return wait_pred(); });
    }

    bool wait_until(std::chrono::steady_clock::time_point t) {
        std::unique_lock lock(m_mutex);
        return m_cvar.wait_until(lock, t, [&] { return wait_pred(); });
    }

    bool wait(timeout t) {
        if (t.is_infinite()) {
            return wait();
        }
        else {
            return wait_for(t.duration);
        }
    }
};

PRAGMA_WARNING_POP

} // namespace pex
