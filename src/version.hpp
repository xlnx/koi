#pragma once

#if ( __cplusplus > 201703L )  // c++20
#define CO_CXX_GE_20
#endif

#if ( __cplusplus > 201402L )  // c++17
#define CO_CXX_GE_17
#endif

#if ( __cplusplus > 201103L )  // c++14
#define CO_CXX_GE_14
#endif

#if ( __cplusplus >= 201103L )  // c++11
#define CO_CXX_GE_11
#endif
