# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_executable(pex-example-hello-world hello-world.cpp)
target_link_libraries(pex-example-hello-world pex::pex)
set_target_properties(pex-example-hello-world PROPERTIES FOLDER example)
