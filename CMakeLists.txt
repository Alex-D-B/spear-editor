cmake_minimum_required(VERSION 3.10)

project(Spear)

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}") 
  message(FATAL_ERROR "In-source builds are disabled.\n"
    "Create a subdirectory `build/` and use `cmake ..` inside it.\n")
endif()

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function")
set(CMAKE_CXX_LINK_FLAGS "${CMAKD_CXX_LINK_FLAGS} -lncurses")

find_package(Catch2 3 REQUIRED)

add_subdirectory(src)
add_executable(spear src/main.cpp)
target_link_libraries(spear PUBLIC src_library)

add_subdirectory(tests)
# Run suite of tests.
add_executable(test tests/tests.cpp)
target_compile_options(test PRIVATE -g -O0 -D DEBUG_MODE=1)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain test_library src_library_debug)
# Target for running miscelaneous code for debugging failures.
add_executable(debug tests/debug.cpp)
target_link_libraries(debug PRIVATE test_library src_library_debug)
target_compile_options(debug PRIVATE -g -O0)
