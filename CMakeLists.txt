cmake_minimum_required(VERSION 3.1)

project(co)
set(CMAKE_CXX_STANDARD 20)

include_directories("${CMAKE_SOURCE_DIR}/src/")

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

file(GLOB test_sources 
"${CMAKE_SOURCE_DIR}/tests/**/*.cc"
"${CMAKE_SOURCE_DIR}/tests/*.cc"
)

add_compile_options(-fcoroutines-ts)

add_executable(test_all ${test_sources})
target_link_libraries(test_all GTest::GTest GTest::Main)

GTEST_ADD_TESTS(test_all "" AUTO)
