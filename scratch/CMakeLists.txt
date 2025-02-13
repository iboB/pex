# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_executable(pex-scratch
    scratch.cpp
)
target_link_libraries(pex-scratch pex::pex)
set_target_properties(pex-scratch PROPERTIES FOLDER scratch)
