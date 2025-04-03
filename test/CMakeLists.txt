# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/doctest-util@0.1.3)
CPMAddPackage(gh:ThrowTheSwitch/Unity@2.6.1)
CPMAddPackage(gh:iboB/b_stacktrace@0.23)
set_target_properties(doctest PROPERTIES FOLDER test)
set_target_properties(doctest-main PROPERTIES FOLDER test)
set_target_properties(unity PROPERTIES FOLDER test)

add_library(doctest-unity STATIC
    doctest-unity-main.cpp
)
set_target_properties(doctest-unity PROPERTIES FOLDER test)
target_link_libraries(doctest-unity PUBLIC
    unity::framework
    doctest::headers
)

macro(pex_x_test test)
    icm_add_test(
        NAME pex-x-${test}
        TARGET test-pex-x-${test}
        LIBRARIES
            doctest-unity
            pex::pex
        SOURCES
            x-${test}.c
            x-${test}.cpp
        FOLDER test
    )
endmacro()

macro(pex_c_test test)
    icm_add_test(
        NAME pex-c-${test}
        TARGET test-pex-c-${test}
        LIBRARIES
            unity::framework
            pex::pex
        SOURCES
            ${ARGN}
        FOLDER test
    )
endmacro()

macro(pex_cxx_test test)
    add_doctest_lib_test(${test} pex ${ARGN})
endmacro()

macro(pex_test test)
    pex_cxx_test(${test} t-${test}.cpp)
endmacro()

pex_test(multi_thread_runner)
pex_test(coro)
pex_cxx_test(coro-stack t-coro-stack.cpp LIBRARIES b_stacktrace::b_stacktrace)
