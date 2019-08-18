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
struct Promise final : future::promise::Eager<T>
{
	using Output = typename NormOut<T()>::type;
	using Handle = coroutine_handle<Promise>;

	struct Async : Future<Output>
	{
		using promise_type = Promise;

		PollState poll() override
		{
			auto &promise = h.promise();
			switch ( auto poll = promise.curr->poll() ) {
			case PollState::Ok:
				promise.curr = nullptr;
				if ( !h.done() ) {
					h.resume();
					if ( promise.curr ) {
						return PollState::Pending;
					}
				}
			default: return poll;
			}
		}
		Output get() override { return h.promise().value(); }

		Async( Handle h ) :
		  h( h ) {}

	private:
		Handle h;
	};

	Async get_return_object() noexcept { return Handle::from_promise( *this ); }
	void set_current_future( Future<> &fut ) { curr = &fut; }

private:
	Future<> *curr = nullptr;
	friend struct Async;
};

template <typename T = void>
using Async = typename Promise<T>::Async;

}  // namespace _

using _::Async;

}  // namespace koi::future::utils

// KOI_FUTURE_STD_HOOK_T(::koi::future::utils::Async )
