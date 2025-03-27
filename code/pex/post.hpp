// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "task.hpp"
#include "strand.hpp"

namespace pex {

PEX_API void post(const strand& s, task f);

class context;
PEX_API void post(context& ctx, task f);

} // namespace pex
