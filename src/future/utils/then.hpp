#pragma once

#include "lazy.hpp"
#include "poll_fn.hpp"
#include <utils/option.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
struct ThenImpl
{
	template <typename A, typename F>
	static auto then( A &&self, F &&fn )
	{
		using B = typename InvokeResultOf<F>::type;
		static_assert( is_base_of<Future<>, B>::value,
					   "then() fn must return some future" );

		using Output = typename B::Output;

		auto first = poll_fn<B>(
		  [fn = normalize<typename A::Output>( std::forward<F>( fn ) ),
		   self = std::forward<A>( self )]( Option<B> &_ ) mutable -> bool {
			  if ( self.poll() ) {
				  _ = fn( std::move( self.get() ) );
				  return true;
			  }
			  return false;
		  } );

		return poll_fn<Output>(
		  [step = 0,
		   first = std::move( first ),
		   second = Option<B>()]( Option<Output> &_ ) mutable -> bool {
			  while ( true ) {
				  switch ( step ) {
				  case 0:
					  if ( !first.poll() ) return false;
					  break;
				  case 1:
					  if ( second.value().poll() ) {
						  _ = std::move( second.value().get() );
						  return true;
					  }
					  return false;
				  default: throw 0;
				  }
				  second = std::move( first.get() );
				  ++step;
			  }
		  } );
	}
};

template <bool X = true>
struct ThenFutImpl : ThenImpl
{
	template <typename Self, typename F>
	static auto then( Self &&self, F &&fn )
	{
		return ThenImpl::then(
		  std::forward<Self>( self ),
		  [fn = std::forward<F>( fn )]( typename Self::Output &&_ ) {
			  return lazy(
				[fn = std::move( fn ),
				 _ = std::move( _ )]() mutable {
					return fn( std::move( _ ) );
				} );
		  } );
	}
};

template <>
struct ThenFutImpl<> : ThenImpl
{
};

template <typename R>
struct ThenFut : ThenFutImpl<is_base_of<Future<>, R>::value>
{
};

template <typename F>
struct Then : ThenFut<typename InvokeResultOf<F>::type>
{
};

template <typename Self, typename F>
auto then( Self &&self, F &&fn )
{
	auto norm = normalize<typename Self::Output>( std::forward<F>( fn ) );
	return Then<decltype( norm )>::then( std::forward<Self>( self ), std::move( norm ) );
}

}  // namespace _

}  // namespace utils

}  // namespace future

}  // namespace koi
