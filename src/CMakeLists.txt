file(GLOB_RECURSE SOURCES *.cpp)
add_library(src_library ${SOURCES})
target_include_directories(src_library PUBLIC ${CMAKE_SOURCE_DIR}/include)