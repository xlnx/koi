cmake_minimum_required(VERSION 3.1)

project(co)
# set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# set(CMAKE_CXX_STANDARD 20)

include_directories("${CMAKE_SOURCE_DIR}/src/")

enable_testing()
cmake_policy(SET CMP0057 NEW)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

file(GLOB test_sources 
"${CMAKE_SOURCE_DIR}/tests/**/*.cc"
"${CMAKE_SOURCE_DIR}/tests/*.cc"
)

add_executable(test_all ${test_sources})
target_link_libraries(test_all GTest::GTest GTest::Main)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options(-std=c++2a)
    add_compile_options(-fcoroutines-ts -stdlib=libc++)
    target_link_libraries(test_all c++ c++abi)
elseif(TRUE)
endif()

gtest_add_tests(test_all "" AUTO)
