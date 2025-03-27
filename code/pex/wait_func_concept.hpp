// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <system_error>
#include <concepts>

namespace pex {
template <typename T>
concept wait_func_class = std::invocable<T, const std::error_code&>;
} // namespace pex
