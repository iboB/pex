// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "strand.hpp"
#include <memory>

namespace pex {

class context_work_guard;

class PEX_API context {
public:
    context();
    ~context();

    context(const context&) = delete;
    context& operator=(const context&) = delete;

    void run();
    void stop();
    bool stopped() const;
    void restart();

    context_work_guard make_work_guard();

    strand make_strand();

    struct impl;
    impl& _impl() { return *m_impl; }
private:
    std::unique_ptr<impl> m_impl;
};

} // namespace pex
