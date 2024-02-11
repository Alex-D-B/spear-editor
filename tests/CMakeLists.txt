file(GLOB_RECURSE SOURCES *.cpp)
add_library(test_library ${SOURCES})
target_include_directories(test_library PUBLIC ${CMAKE_SOURCE_DIR}/include)