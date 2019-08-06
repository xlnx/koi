cmake_minimum_required(VERSION 3.1)

project(co)

include_directories("${CMAKE_SOURCE_DIR}/src/")

enable_testing()
cmake_policy(SET CMP0057 NEW)

file(GLOB test_sources 
"${CMAKE_SOURCE_DIR}/tests/**/*.cc"
"${CMAKE_SOURCE_DIR}/tests/*.cc"
)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options(-std=c++17 -stdlib=libc++)
    # add_compile_options(-std=c++2a -fcoroutines-ts -stdlib=libc++)
    link_libraries(c++ c++abi)
elseif(TRUE)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest")
    
add_executable(test_all ${test_sources})
target_link_libraries(test_all gtest_main gtest pthread)

include(GoogleTest)
gtest_add_tests(test_all "" AUTO)
