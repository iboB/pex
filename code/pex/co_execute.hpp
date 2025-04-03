// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "co_spawn.hpp"
#include "context.hpp"
#include "context_work_guard.hpp"

namespace pex {

template <typename T>
T co_execute(coro<T> c) {
    using result_t = typename coro<T>::coro_result;

    result_t result = itlib::unexpected();

    context ctx;
    context_work_guard guard(ctx);
    co_spawn(ctx, [&]() -> coro<void> {
        typename coro<T>::result_awaitable a(c.take_handle());
        result = co_await a;
        guard.reset();
    }());
    ctx.run();

    if (result) {
        return std::move(result).value();
    }
    else {
        std::rethrow_exception(result.error());
    }
}

} // namespace pex
