#pragma once

#include <experimental/coroutine>

namespace co::future
{

namespace _
{

#define CO_FUTURE_STD_HOOK(Future)                          \
    namespace std::experimental                             \
    {                                                       \
    template <typename... Args>                             \
    struct coroutine_traits<Future, Args...>                \
    {                                                       \
        using promise_type = typename Future::promise_type; \
    };                                                      \
    } // namespace std::experimental

#define CO_FUTURE_STD_HOOK_T(Future)                           \
    namespace std::experimental                                \
    {                                                          \
    template <typename ...T, typename... Args>                    \
    struct coroutine_traits<Future<T...>, Args...>                \
    {                                                          \
        using promise_type = typename Future<T...>::promise_type; \
    };                                                         \
    } // namespace std::experimental

} // namespace _

} // namespace co::future