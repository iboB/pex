// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <pex/coro.hpp>
#include <pex/strand_wobj.hpp>
#include <pex/timer_wobj.hpp>
#include <pex/context.hpp>
#include <pex/context_work_guard.hpp>
#include <pex/co_spawn.hpp>
#include <pex/co_execute.hpp>
#include <pex/thread_runner.hpp>
#include <doctest/doctest.h>

class worker {
    pex::timer_wobj m_wobj;
    pex::strand_wobj& m_notify;
public:
    worker(pex::strand s, pex::strand_wobj& notify)
        : m_wobj(s)
        , m_notify(notify)
    {
        co_spawn(s, run());
    }

    int a, b;
    int result;
    pex::coro<void> run() {
        while (true) {
            auto notified = co_await m_wobj.wait(pex::timeout::after_ms(100));
            if (!notified) continue;
            result = a + b;
            m_notify.notify_one();
            if (result == 0) co_return;
        }
    }

    void notify() {
        m_wobj.notify_one();
    }
};

pex::coro<int> test() {
    pex::context ctx;
    auto wg = ctx.make_work_guard();
    pex::thread_runner runner(ctx, 1, "worker");

    pex::strand_wobj wobj(co_await pex::executor{});
    worker wrk(pex::make_strand(ctx), wobj);

    wrk.a = 5;
    wrk.b = 10;
    wrk.notify();
    CHECK(co_await wobj.wait());
    CHECK(wrk.result == 15);

    wrk.a = 20;
    wrk.b = 30;
    wrk.notify();
    CHECK(co_await wobj.wait());
    CHECK(wrk.result == 50);

    wrk.a = 0;
    wrk.b = 0;
    wrk.notify();
    CHECK(co_await wobj.wait());

    wg.reset();
    co_return wrk.result;
}

TEST_CASE("worker") {
    CHECK(co_execute(test()) == 0);
}
