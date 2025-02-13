// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "strand.hpp"
#include "ufunc.hpp"
#include <chrono>
#include <cstdint>
#include <memory>

namespace pex {

class timer;
using timer_ptr = std::unique_ptr<timer>;

class PEX_API timer {
public:
    virtual ~timer();

    timer(const timer&) = delete;
    timer& operator=(const timer&) = delete;

    virtual void expire_after(std::chrono::milliseconds t_from_now) = 0;

    virtual void cancel() = 0;
    virtual void cancel_one() = 0;

    using cb_t = ufunc<void(bool cancelled)>;
    virtual void addCallback(cb_t cb) = 0;

    timer_ptr create(const strand& s);
private:
    // sealed interface
    timer();
    friend struct timer_impl;
};

} // namespace pex
