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
    using result_type = typename coro<T>::result_type;

    result_type result = itlib::unexpected();

    context ctx;
    context_work_guard guard(ctx);
    co_spawn(ctx, [&]() -> coro<void> {
        result = co_await typename coro<T>::result_awaitable(c.take_handle());
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
