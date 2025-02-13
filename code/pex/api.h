// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <splat/symbol_export.h>

#if PEX_SHARED
#   if BUILDING_PEX
#       define PEX_API SYMBOL_EXPORT
#   else
#       define PEX_API SYMBOL_IMPORT
#   endif
#else
#   define PEX_API
#endif
