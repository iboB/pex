# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(pex
    LANGUAGES C CXX
)

# cpm
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.4)

list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

set(ICM_DEV_C_STANDARD 11)
set(ICM_DEV_CXX_STANDARD 20)

include(icm_dev_mode)

#######################################
# options

option(BUILD_SHARED_LIBS "${PROJECT_NAME}: build shared libraries" ON)
option(PEX_BUILD_TESTS "${PROJECT_NAME}: enable testing and build tests" ${ICM_DEV_MODE})
option(PEX_BUILD_EXAMPLES "${PROJECT_NAME}: build examples" ${ICM_DEV_MODE})
option(PEX_BUILD_TUTORIALS "${PROJECT_NAME}: build tutorials" ${ICM_DEV_MODE})
option(PEX_BUILD_BENCH "${PROJECT_NAME}: build benchmarks" ${ICM_DEV_MODE})
option(PEX_BUILD_SCRATCH "${PROJECT_NAME}: build scratch project (dev experiments)" ${ICM_DEV_MODE})
mark_as_advanced(PEX_BUILD_SCRATCH)

#######################################
# packages

if(NOT TARGET Boost::boost)
    # only add boost if not added from the outside
    CPMAddPackage(gh:iboB/boost-trim@1.85.0)
endif()

if(NOT TARGET Boost::asio)
    # asio is header only, so this is fine
    add_library(Boost::asio ALIAS Boost::boost)
endif()

#######################################
# code
add_subdirectory(code)

if(PEX_BUILD_SCRATCH)
    add_subdirectory(scratch)
endif()

if(PEX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(PEX_BUILD_BENCH)
    add_subdirectory(bench)
endif()

if(PEX_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
