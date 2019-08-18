#pragma once

#include "promise.hpp"
#include "std_hook.hpp"
#include <future/future.hpp>
#include <utils/normalize.hpp>

namespace koi::future::utils
{
namespace _
{
using namespace std;
using namespace experimental;
using namespace traits::concepts;
using namespace koi::utils;

template <typename T = void>
struct Async;

template <typename Promise>
struct AsyncBase
{
	using promise_type = Promise;

protected:
	AsyncBase( Future<> &_ ) :
	  _( _ )
	{
	}

protected:
	Future<> &_;
};

template <typename F, typename T = void>
struct Promise final : future::promise::Eager<T>
{
	struct Coroutine : koi::Future<>
	{
		PollState poll() override
		{
			switch ( auto poll = curr->poll() ) {
			case PollState::Ok:
				curr = nullptr;
				if ( !h.done() ) {
					h.resume();
					if ( curr ) {
						return PollState::Pending;
					}
				}
			default: return poll;
			}
		}

		Coroutine( coroutine_handle<Promise> h ) :
		  h( h ) {}

	public:
		Future<> *curr = nullptr;
		coroutine_handle<Promise> h;
	};

	F get_return_object() noexcept
	{
		return F( _ );
	}

	void set_current_future( Future<> &fut )
	{
		_.curr = &fut;
	}

private:
	Coroutine _ = coroutine_handle<Promise>::from_promise( *this );
};

template <typename T>
struct Async : Future<T>, AsyncBase<Promise<Async<T>, T>>
{
	PollState poll() override { return this->_.poll(); }

private:
	using AsyncBase<Promise<Async, T>>::AsyncBase;
	friend struct Promise<Async<T>, T>;
};

template <>
struct Async<> : Future<Void>, AsyncBase<Promise<Async<>, Void>>
{
	PollState poll() override { return this->_.poll(); }
	Void get() override { return Void{}; }

private:
	using AsyncBase<Promise<Async<>, Void>>::AsyncBase;
	friend struct Promise<Async<>, Void>;
};

}  // namespace _

using _::Async;

}  // namespace koi::future::utils

KOI_FUTURE_STD_HOOK_T(::koi::future::utils::Async )
