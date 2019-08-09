#pragma once

#include "promise.hpp"
#include "std_hook.hpp"
#include <future/future.hpp>

namespace koi::future::utils
{
namespace _
{
using namespace std;
using namespace experimental;
using namespace traits::concepts;

template <typename T = void>
struct Async;

template <typename Promise>
struct AsyncBase
{
	using promise_type = Promise;

protected:
	AsyncBase( coroutine_handle<Promise> _ ) :
	  _( _ ) {}

protected:
	coroutine_handle<Promise> _;
};

template <typename F, typename T = void>
struct Promise final : future::promise::Lazy<T>
{
	using Future = F;
	F get_return_object() noexcept
	{
		return F( coroutine_handle<Promise>::from_promise( *this ) );
	}
};

template <>
struct Async<> : Future<>, AsyncBase<Promise<Async<>>>
{
	void poll() override { this->_.resume(); }
	void await_resume() noexcept { return this->poll(); }

private:
	using AsyncBase<Promise<Async>>::AsyncBase;

private:
	friend struct Future<>;
	friend struct Promise<Async>;
};

template <typename T>
struct Async final : Future<T>, AsyncBase<Promise<Async<T>, T>>
{
	void poll() override { this->_.resume(); }
	T poll_result() override
	{
		this->_.resume();
		return std::move( this->_.promise().value() );
	}
	void await_resume() noexcept { return this->resume(); }

private:
	using AsyncBase<Promise<Async, T>>::AsyncBase;

private:
	friend struct Future<>;
	friend struct Promise<Async, T>;
};

}  // namespace _

using _::Async;

}  // namespace koi::future::utils

KOI_FUTURE_STD_HOOK_T(:
  : koio::future::utils::Async )
