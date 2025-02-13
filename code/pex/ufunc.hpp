// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <itlib/ufunction.hpp>

namespace pex {
// C++23: replace with std::move_only_function
template <typename F>
using ufunc = itlib::ufunction<F>;
} // namespace pex
