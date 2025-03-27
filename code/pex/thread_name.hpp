// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"

#include <thread>
#include <string_view>
#include <string>

namespace pex {
// Debug helpers which give names to threads so that they are easily identifiable in the debugger
// Return 0 on success, non-zero otherwise
PEX_API int set_thread_name(std::thread& t, std::string_view name) noexcept;
PEX_API int set_this_thread_name(std::string_view name) noexcept;
PEX_API std::string get_thread_name(std::thread& t) noexcept;
PEX_API std::string get_this_thread_name() noexcept;
} // namespace pex
