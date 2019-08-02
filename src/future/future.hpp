#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include "promise.hpp"
#include "std_hook.hpp"
#include <traits/concepts.hpp>

namespace co::runtime::_
{
template <typename F>
void spawn( F &&future );
}

namespace co::future
{
namespace _
{
using namespace std;
using namespace experimental;
using namespace traits::concepts;

template <typename F = void>
struct Future;

template <typename T>
struct Async;

template <>
struct Future<void>
{
	void poll() const { _.resume(); }

	Future<void>( coroutine_handle<> const &_ ) :
	  _( _ )
	{
	}

protected:
	mutable coroutine_handle<> _;
};

template <template <typename> typename X, typename T>
struct Future<X<T>> : Future<>, ExplicitCopy
{
	struct Promise final : future::promise::Lazy<T>
	{
		X<T> get_return_object() noexcept
		{
			return X<T>( coroutine_handle<Promise>::from_promise( *this ) );
		}
	};
	using promise_type = Promise;

	bool await_ready() const noexcept { return false; }

protected:
	using Future<>::Future;

protected:
	coroutine_handle<Promise> &handle() const
	{
		return reinterpret_cast<coroutine_handle<Promise> &>( _ );
	}

private:
	using Future<>::_;
};

template <typename T>
struct Async final : Future<Async<T>>
{
	void await_suspend( coroutine_handle<> _ ) const noexcept
	{
		runtime::_::spawn( Future<>( _ ) );
	}
	T await_resume() const noexcept
	{
		auto &_ = this->handle();
		_.resume();
		return std::move( _.promise().value() );
	}
	using Future<Async>::Future;
};

}  // namespace _

using _::Async;
using _::Future;

}  // namespace co::future

CO_FUTURE_STD_HOOK_T(::co::future::Async )

namespace co
{
using future::Async;
using future::Future;

}  // namespace co
