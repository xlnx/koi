#pragma once

#include <iostream>

#include "promise.hpp"
#include "std_hook.hpp"

namespace co::future
{

namespace _
{

using namespace std;
using namespace experimental;

template <typename T = void>
struct Future
{
    struct Promise : future::promise::Eager
    {
        Future<T> get_return_object()
        {
            LOG_CHECKPOINT();
            return Future<T>{};
        }
        // auto final_suspend() noexcept
        // {
        //     LOG_CHECKPOINT();
        //     return suspend_never{};
        // }
        // auto return_void() noexcept {}
        auto return_value(const T &value) noexcept
        {
            LOG_CHECKPOINT();
        }
    };

    using promise_type = Promise;

    Future() { LOG_CHECKPOINT(); }
    ~Future() { LOG_CHECKPOINT(); }
};

} // namespace _

using _::Future;

} // namespace co::future

CO_FUTURE_STD_HOOK_T(::co::future::Future)
