// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "ufunc.hpp"
#include "strand.hpp"

namespace pex {

PEX_API void post(const strand& s, ufunc<void()> f);

class context;
PEX_API void post(context& ctx, ufunc<void()> f);

} // namespace pex
