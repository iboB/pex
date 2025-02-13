# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_add_lib)

icm_add_lib(pex PEX)
add_library(pex::pex ALIAS pex)
target_include_directories(pex INTERFACE .)

CPMAddPackage(gh:iboB/splat@1.3.3)

target_link_libraries(pex PUBLIC
    splat::splat
    Boost::asio
)

target_sources(pex PRIVATE
    pex/api.h
    pex/mylib.h
    pex/mylib.c
    pex/mylib.hpp
    pex/mylib.cpp
)

