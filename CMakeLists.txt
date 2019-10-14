cmake_minimum_required(VERSION 3.1)
project(koi)

find_package(Git)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive)
include(scripts/external.cmake)
include(export.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    # add_compile_options(-std=c++17 -stdlib=libc++)
    add_compile_options(-std=c++2a -fcoroutines-ts -stdlib=libc++)
    link_libraries(c++ c++abi)
elseif(TRUE)
endif()

option(KOI_BUILD_TESTS "build tests" OFF)
if (KOI_BUILD_TESTS)
  enable_testing()
  cmake_policy(SET CMP0057 NEW)

  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest")
  
  file(GLOB_RECURSE test_sources 
	tests/**/*.cc
	tests/*.cc
  )
  add_executable(koi_test_all ${test_sources})
  vm_target_dependency(koi_test_all koi PRIVATE)
  target_link_libraries(koi_test_all gtest_main gtest pthread uv)
  
  include(GoogleTest)
  gtest_add_tests(koi_test_all "" AUTO)
endif()
