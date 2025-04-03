// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "post.hpp"
#include <itlib/expected.hpp>
#include <coroutine>
#include <stdexcept>
#include <cassert>
#include <memory>

namespace pex {

namespace impl {
template <typename T>
using coro_result = itlib::expected<T, std::exception_ptr>;

template <typename T, typename Self>
struct ret_promise_helper {
    void return_value(T value) noexcept {
        auto& self = static_cast<Self&>(*this);
        assert(self.m_result); // can't return value without a result to store it in
        *self.m_result = std::move(value);
    }
};
template <typename Self>
struct ret_promise_helper<void, Self> {
    void return_void() noexcept {
        auto& self = static_cast<Self&>(*this);
        if (self.m_result) {
            // m_result may be null in the root coroutine if it's void
            *self.m_result = {};
        }
    }
};

} // namespace impl

template <typename Ret>
struct coro {
    using return_type = Ret;

    struct promise_type;
    using handle = std::coroutine_handle<promise_type>;

    using coro_result = impl::coro_result<Ret>;

    struct promise_type : impl::ret_promise_helper<Ret, promise_type> {
        coro get_return_object() noexcept {
            return coro{handle::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept { return {}; }

        auto final_suspend() noexcept {
            // what we do here is that we make the top coroutine have an eager return (not-suspending)
            // the top coroutine must be detached from its object (on co_spawn)
            // the others wont: they will be suspended here and destroyed from the caller
            struct final_awaitable {
                std::coroutine_handle<> prev;
                bool await_ready() const noexcept {
                    // eager when no prev
                    return !prev;
                }
                std::coroutine_handle<> await_suspend(handle) noexcept {
                    // resume with caller
                    return prev;
                }
                void await_resume() noexcept {}
            };
            return final_awaitable{m_prev};
        }

        void unhandled_exception() noexcept {
            if (!m_result) {
                std::terminate(); // can't throw exceptions from a naked top coroutine
            }
            *m_result = itlib::unexpected(std::current_exception());
        }

        strand m_executor;
        std::coroutine_handle<> m_prev = nullptr;

        // points to the result in the awaitable which is on the stack
        // null if this is the top coroutine
        coro_result* m_result = nullptr;
    };

    coro() noexcept = default;
    coro(coro&& other) noexcept : m_handle(other.take_handle()) {}
    coro& operator=(coro&& other) noexcept {
        if (m_handle) {
            m_handle.destroy();
        }
        m_handle = other.take_handle();
        return *this;
    }
    ~coro() {
        if (m_handle) {
            m_handle.destroy();
        }
    }

    handle take_handle() noexcept {
        return std::exchange(m_handle, nullptr);
    }

    struct awaitable {
        handle hcoro;

        awaitable(handle h) noexcept : hcoro(h) {}

        // instead of making optional of expected, we can use the value error=nullptr to indicate that
        // the result is empty (hacky, but works)
        coro_result result = itlib::unexpected();

        bool await_ready() const noexcept { return false; }

        template <typename CallerPromise>
        std::coroutine_handle<> await_suspend(std::coroutine_handle<CallerPromise> caller) noexcept {
            hcoro.promise().m_result = &result;
            hcoro.promise().m_executor = caller.promise().m_executor;
            hcoro.promise().m_prev = caller;
            return hcoro;
        }

        Ret await_resume() noexcept(false) {
            if (result) {
                return std::move(result).value();
            }
            else {
                std::rethrow_exception(result.error());
            }
        }
    };

    awaitable operator co_await() {
        return {m_handle};
    }

    explicit operator bool() const noexcept {
        return !!m_handle;
    }

private:
    handle m_handle;
    coro(handle h) noexcept : m_handle(h) {}
};

// awaitable to get the coroutine's executor from the coroutine itself
// co_await executor{}
struct executor {
    strand m_executor;

    // awaitable interface
    bool await_ready() const noexcept { return false; }
    template <typename PromiseType>
    bool await_suspend(std::coroutine_handle<PromiseType> h) noexcept {
        m_executor = h.promise().m_executor;
        return false;
    }
    strand await_resume() noexcept { return std::move(m_executor); }
};

inline void co_spawn(strand ex, coro<void> c) {
    auto h = c.take_handle();
    h.promise().m_executor = ex;
    post(ex, [h] {
        h.resume();
    });
}

inline void co_spawn(context& ctx, coro<void> c) {
    co_spawn(make_strand(ctx), std::move(c));
}

} // namespace pex
