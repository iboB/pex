// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <pex/context.hpp>
#include <pex/strand.hpp>
#include <pex/timer.hpp>
#include <iostream>
#include <chrono>

int main() {
    using namespace std::chrono_literals;

    pex::context ctx;
    auto strand = make_strand(ctx);
    auto timer = pex::timer::create(strand);
    timer->expire_after(1s);
    timer->add_wait_cb([](const std::error_code& ec) {
        std::cout << ec.category().name() << std::endl;
        std::cout << ec.value() << std::endl;
        std::cout << ec.message() << std::endl;
    });
    timer->cancel();
    ctx.run();
}
