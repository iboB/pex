// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <pex/coro.hpp>
#include <pex/context.hpp>
#include <pex/co_spawn.hpp>
#include <doctest/doctest.h>

using namespace pex;

generator<int> ints() {
    co_yield 5;
}

coro<void> trivial() {
    auto g = ints();
    auto result = co_await g.next();
    CHECK(result);
    CHECK(*result == 5);
    result = co_await g.next();
    CHECK(!result);
}

TEST_CASE("trivial") {
    context ctx;
    co_spawn(ctx, trivial());
    ctx.run();
}
