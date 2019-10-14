#pragma once

#include "poll_fn.hpp"
#include <utils/normalize.hpp>
#include <utils/result.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits;
using namespace koi::utils;

struct IntoSelf
{
	template <typename F>
	static auto into_future( F &&fn )
	{
		return std::forward<F>( fn );
	}
};

template <typename I, bool X>
struct IntoFutureImpl : IntoSelf
{
	template <typename F>
	static auto into_future( F &&fn )
	{
		return IntoSelf::into_future(
		  [fn = std::forward<F>( fn )]( I &&_ ) {
			  return lazy(
				[fn = std::move( fn ),
				 _ = std::move( _ )]() mutable {
					return fn( std::move( _ ) );
				} );
		  } );
	}
};

template <typename I>
struct IntoFutureImpl<I, true> : IntoSelf
{
};

template <typename I, typename R, bool X>
struct IntoFutureResultable : IntoFutureImpl<I, is_base_of<Future<>, R>::value>
{
};

template <typename I, typename T, typename E, bool FT = false, bool FE = false>
struct IntoFutureResultableImpl : IntoFutureResultable<I, Result<T, E>, false>
{
};

template <typename I, typename T, typename E>
struct IntoFutureResultableImpl<I, T, E, true, false>
{
	template <typename F>
	static auto into_future( F &&fn )
	{
		using T1 = typename T::Output;
		using O = Result<T1, E>;
		return IntoSelf::into_future(
		  [fn = std::forward<F>( fn )]( I &&_ ) {
			  return poll_fn<O>(
				[res = fn( std::move( _ ) )]( Option<O> &_ ) mutable -> PollState {
					if ( res.is_ok() ) {
						switch ( auto poll = res.ok().poll() ) {
						case PollState::Ok: _ = O::Ok( std::move( res.ok().get() ) );
						default: return poll;
						}
					} else {
						_ = O::Err( std::move( res.err() ) );
						return PollState::Ok;
					}
				} );
		  } );
	}
};

template <typename I, typename T, typename E>
struct IntoFutureResultableImpl<I, T, E, false, true>
{
	template <typename F>
	static auto into_future( F &&fn )
	{
		using T1 = typename T::Output;
		using O = Result<T1, E>;
		return IntoSelf::into_future(
		  [fn = std::forward<F>( fn )]( I &&_ ) {
			  return poll_fn<O>(
				[res = fn( std::move( _ ) )]( Option<O> &_ ) mutable -> PollState {
					if ( res.is_ok() ) {
						_ = O::Ok( std::move( res.ok() ) );
						return PollState::Ok;
					} else {
						switch ( auto poll = res.err().poll() ) {
						case PollState::Ok: _ = O::Err( std::move( res.err().get() ) );
						default: return poll;
						}
					}
				} );
		  } );
	}
};

template <typename I, typename T, typename E>
struct IntoFutureResultableImpl<I, T, E, true, true>
{
	// static_assert( false, "unimplemented Result<Future<>, Future<>>" );
};

template <typename I, typename T, typename E>
struct IntoFutureResultable<I, Result<T, E>, true> : IntoFutureResultableImpl<
													   I, T, E,
													   is_base_of<Future<>, T>::value,
													   is_base_of<Future<>, E>::value>
{
};

struct IntoFuture
{
	template <typename F>
	static auto into_future( F &&fn )
	{
		using Args = typename ArgumentTypeOf<F>::type;
		using Out = typename InvokeResultOf<F>::type;
		using In = decltype( std::get<0>( declval<Args>() ) );
		using Next = IntoFutureResultable<In, Out, true>;
		return Next::into_future( std::forward<F>( fn ) );
	}
};

template <typename D, typename F>
auto into_future( F &&fn )
{
	auto norm = normalize<D>( std::forward<F>( fn ) );
	return IntoFuture::into_future( std::move( norm ) );
}

}  // namespace _

using _::into_future;

}  // namespace utils

}  // namespace future

}  // namespace koi
