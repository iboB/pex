// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <memory>

namespace pex {

class context;

class PEX_API context_work_guard {
public:
    context_work_guard();
    explicit context_work_guard(context&);
    ~context_work_guard();

    context_work_guard(context_work_guard&&) noexcept;
    context_work_guard& operator=(context_work_guard&&) noexcept;

    explicit operator bool() const noexcept { return !!m_impl; }

    void reset();

private:
    struct impl;
    std::unique_ptr<impl> m_impl;
};

} // namespace pex
