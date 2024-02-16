file(GLOB_RECURSE SOURCES *.cpp)

# Release
add_library(src_library ${SOURCES})
target_include_directories(src_library PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Tests/Debug
add_library(src_library_debug ${SOURCES})
target_include_directories(src_library_debug PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(src_library_debug PRIVATE -g -O0)