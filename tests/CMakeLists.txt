# Copyright 2018 Tymoteusz Blazejczyk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if (NOT VERILATOR_FOUND)
    message(WARNING "Tests enabled but cannot find Verilator")
    return()
endif()

if (NOT GTEST_FOUND)
    message(WARNING "Tests enabled but cannot find Google Test")
    return()
endif()

if (NOT SVUNIT_FOUND)
    message(WARNING "Tests enabled but cannot find SVUnit")
    return()
endif()

set(HDL_SYNTHESIZABLE FALSE)

add_subdirectory(logic)
