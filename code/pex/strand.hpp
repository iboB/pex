// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <memory>

namespace pex {
struct strand_impl; // opaque
using strand = std::shared_ptr<strand_impl>;

class context;
PEX_API strand make_strand(context& ctx);
} // namespace pex
