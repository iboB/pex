// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <pex/coro.hpp>
#include <pex/context.hpp>
#include <pex/co_spawn.hpp>
#include <doctest/doctest.h>

#define B_STACKTRACE_IMPL
#include <b_stacktrace.h>

using namespace pex;

coro<int> depth() {
    auto trace = b_stacktrace_get();
    auto ret = b_stacktrace_depth(trace);
    free(trace);
    co_return ret;
}

coro<void> stack_flatten_test() {
    // eagerly resuming on return does not increase the stack depth
    // it is expected to be flattened as per https://eel.is/c++draft/expr.await#note-1
    const auto d = co_await depth();
    CHECK(d == co_await depth());
    CHECK(d == co_await depth());
    CHECK(d == co_await depth());
}

TEST_CASE("stack flatten") {
    context ctx;
    co_spawn(ctx, stack_flatten_test());
    ctx.run();
}
