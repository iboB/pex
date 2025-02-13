// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/steady_timer.hpp>
#include <iostream>
#include <chrono>

namespace asio = boost::asio;

int main() {
    using namespace std::chrono_literals;

    asio::io_context ctx;
    auto strand = asio::make_strand(ctx);
    asio::steady_timer timer(strand);
    std::cout << timer.expiry().time_since_epoch().count() << std::endl;
    //timer.expires_after(1s);
    timer.async_wait([](const std::error_code& ec) {
        std::cout << ec.category().name() << std::endl;
        std::cout << ec.value() << std::endl;
        std::cout << ec.message() << std::endl;
    });
    //timer.cancel();
    ctx.run();
}
