#pragma once

#include <iostream>
#include <functional>

#include "promise.hpp"
#include "std_hook.hpp"
#include <traits/concepts.hpp>

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

struct Executor : Dynamic
{
	virtual void spawn( Future<> const &future ) = 0;
	virtual void run( Future<> const& entry ) = 0;
};

struct DefaultExecutor : Executor
{
	static void set( unique_ptr<Executor> &&executor ) { _() = std::move( executor ); }
	static Executor &instance() { return *_(); }

private:
	static unique_ptr<Executor> &_()
	{
		static unique_ptr<Executor> _;
		return _;
	}
};

template <>
struct Future<void>
{
	void poll() const { _.resume(); }

	Future<void>( coroutine_handle<> const &_ ) :
	  _( _ ) {}

protected:
	mutable coroutine_handle<> _;
};

template <template <typename> typename X, typename T>
struct Future<X<T>> : Future<>
{
	struct Promise final : future::promise::Lazy<T>
	{
		X<T> get_return_object() noexcept
		{
			return X<T>( coroutine_handle<Promise>::from_promise( *this ) );
		}
	};
	using promise_type = Promise;

	Future( coroutine_handle<> _ ) :
	  Future<void>( _ ) {}

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
	bool await_ready() const noexcept { return false; }
	void await_suspend( coroutine_handle<> h ) const noexcept
	{
		DefaultExecutor::instance().spawn( Future<>{ h } );
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
using _::DefaultExecutor;
using _::Executor;
using _::Future;

}  // namespace co::future

CO_FUTURE_STD_HOOK_T(::co::future::Async )

namespace co
{

using future::Async;
using future::Future;
using future::Executor;

}
