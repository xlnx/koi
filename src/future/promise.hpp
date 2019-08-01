#pragma once

#include <iostream>
#include <optional>
#include <experimental/coroutine>

#define DEBUG
#include "log.hpp"

namespace co::future::promise
{

namespace _
{

using namespace std;
using namespace experimental;

struct PromiseBase
{
    PromiseBase() {}
    ~PromiseBase() { }

    auto final_suspend() noexcept { return suspend_never{}; }
    auto unhandled_exception() noexcept { std::terminate(); }
};

template <typename T>
struct Promise: PromiseBase
{
    void return_value( T &&value ) noexcept { _ = std::move(value); }
    T value() noexcept { return std::move( _.value() ); }

private:
    optional<T> _;
};

template <>
struct Promise<void>: PromiseBase
{
    void return_void() noexcept {}
    void value() noexcept {}
};

template <typename T>
struct Lazy : Promise<T>
{
    auto initial_suspend() noexcept { return suspend_always{}; }
};

template <typename T>
struct Eager : Promise<T>
{
    auto initial_suspend() noexcept { return suspend_never{}; }
};

} // namespace _

using _::Eager;
using _::Lazy;

} // namespace co::future::promise
