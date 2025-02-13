// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "context.hpp"
#include "context_work_guard.hpp"
#include "post.hpp"
#include "timer.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <itlib/make_ptr.hpp>

namespace asio = boost::asio;

namespace pex {

struct context::impl {
    asio::io_context ctx;
};

struct context_work_guard::impl {
    asio::executor_work_guard<asio::io_context::executor_type> guard;
};

context_work_guard::context_work_guard() = default;
context_work_guard::context_work_guard(context& ctx)
    : m_impl(itlib::make_unique(impl{asio::make_work_guard(ctx._impl().ctx.get_executor())}))
{}
context_work_guard::~context_work_guard() = default;

context_work_guard::context_work_guard(context_work_guard&&) noexcept = default;
context_work_guard& context_work_guard::operator=(context_work_guard&&) noexcept = default;

void context_work_guard::reset() {
    m_impl.reset();
}

struct strand_impl {
    asio::strand<asio::io_context::executor_type> strand;
};

context::context()
    : m_impl(std::make_unique<impl>())
{}

context::~context() = default;

void context::run() {
    m_impl->ctx.run();
}

void context::stop() {
    m_impl->ctx.stop();
}

bool context::stopped() const {
    return m_impl->ctx.stopped();
}

void context::restart() {
    m_impl->ctx.restart();
}

context_work_guard context::make_work_guard() {
    return context_work_guard(*this);
}

strand context::make_strand() {
    return itlib::make_shared(strand_impl{asio::make_strand(m_impl->ctx.get_executor())});
}

strand make_strand(context& ctx) {
    return ctx.make_strand();
}

void post(const strand& s, ufunc<void()> f) {
    asio::post(s->strand, std::move(f));
}

void post(context& ctx, ufunc<void()> f) {
    asio::post(ctx._impl().ctx, std::move(f));
}

} // namespace pex
