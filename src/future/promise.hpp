#pragma once

#include <iostream>
#include <experimental/coroutine>

#define DEBUG
#include "log.hpp"

namespace co::future::promise
{

namespace _
{

using namespace std;
using namespace experimental;

struct Promise
{
    Promise() { LOG_CHECKPOINT(); }
    ~Promise() { LOG_CHECKPOINT(); }

    auto final_suspend() noexcept
    {
        LOG_CHECKPOINT();
        return suspend_never{};
    }
    // auto return_void() noexcept {}
    auto unhandled_exception() noexcept
    {
        LOG_CHECKPOINT();
    }
};

struct Lazy : Promise
{
    auto initial_suspend() noexcept
    {
        LOG_CHECKPOINT();
        return suspend_always{};
    }
};

struct Eager : Promise
{
    auto initial_suspend() noexcept
    {
        LOG_CHECKPOINT();
        return suspend_never{};
    }
};

} // namespace _

using _::Eager;
using _::Lazy;

} // namespace co::future::promise
