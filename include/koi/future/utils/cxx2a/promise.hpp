#pragma once

#include <iostream>
#include <utils/option.hpp>
#include <utils/normalize.hpp>
#include <experimental/coroutine>
#include <traits/concepts.hpp>

namespace koi::future::promise
{
namespace _
{
using namespace std;
using namespace experimental;
using namespace koi::utils;
using namespace traits::concepts;

struct PromiseBase : NoCopy, NoMove
{
	PromiseBase() {}
	~PromiseBase() {}

	auto final_suspend() noexcept { return suspend_never{}; }
	auto unhandled_exception() noexcept { std::terminate(); }
};

template <typename T = void>
struct Promise : PromiseBase
{
	void return_value( T &&value ) noexcept { _ = std::move( value ); }
	T value() noexcept { return std::move( _.value() ); }

private:
	Option<T> _;
};

template <>
struct Promise<> : PromiseBase
{
	void return_void() noexcept {}
	Void value() noexcept { return Void{}; }
};

template <typename T = void>
struct Lazy : Promise<T>
{
	auto initial_suspend() noexcept { return suspend_always{}; }
};

template <typename T = void>
struct Eager : Promise<T>
{
	auto initial_suspend() noexcept { return suspend_never{}; }
};

}  // namespace _

using _::Eager;
using _::Lazy;

}  // namespace koi::future::promise
