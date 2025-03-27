# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_add_lib)

icm_add_lib(pex PEX)
add_library(pex::pex ALIAS pex)
target_include_directories(pex INTERFACE .)

CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.5)

target_link_libraries(pex
    PUBLIC
        splat::splat
        itlib::itlib
    PRIVATE
        Boost::asio
)

target_sources(pex PRIVATE
    pex/api.h

    pex/context.hpp

    pex/pex-asio-impl.cpp
    pex/thread_name.cpp
)

