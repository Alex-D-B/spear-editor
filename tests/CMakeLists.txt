file(GLOB_RECURSE SOURCES *.cpp)
add_library(test_library ${SOURCES})
target_include_directories(test_library PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_library PRIVATE Catch2::Catch2WithMain)
