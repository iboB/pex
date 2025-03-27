# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/picobench@2.07)

add_subdirectory(picobench-main)

macro(pex_benchmark name)
    set(tgt bench-pex-${name})
    add_executable(${tgt})
    set_target_properties(${tgt} PROPERTIES FOLDER bench)
    target_sources(${tgt} PRIVATE ${ARGN})
    target_link_libraries(${tgt} pex::pex picobench-main)
    add_custom_target(
        run-${tgt}
        COMMAND ${tgt}
    )
    set_target_properties(run-${tgt} PROPERTIES FOLDER bench)
endmacro()
