#pragma once

#include "lazy.hpp"
#include "into_future.hpp"
#include <utils/option.hpp>
#include <utils/result.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
struct Then
{
	template <typename A, typename F>
	static auto then( A &&self, F &&fn )
	{
		using B = typename InvokeResultOf<F>::type;
		static_assert( is_base_of<Future<>, B>::value,
					   "then() fn must return some future" );

		using Output = typename B::Output;

		auto first = poll_fn<B>(
		  [fn = std::forward<F>( fn ),
		   self = std::forward<A>( self )]( Option<B> &_ ) mutable -> PollState {
			  switch ( auto poll = self.poll() ) {
			  case PollState::Ok: _ = fn( std::move( self.get() ) );
			  default: return poll;
			  }
		  } );

		return poll_fn<Output>(
		  [step = 0,
		   first = std::move( first ),
		   second = Option<B>()]( Option<Output> &_ ) mutable -> PollState {
			  while ( true ) {
				  switch ( step ) {
				  case 0:
					  switch ( auto poll = first.poll() ) {
					  default: return poll;
					  case PollState::Ok:;
					  }
					  break;
				  case 1:
					  switch ( auto poll = second.value().poll() ) {
					  case PollState::Ok: _ = std::move( second.value().get() );
					  default: return poll;
					  }
				  default: throw 0;
				  }
				  second = std::move( first.get() );
				  ++step;
			  }
		  } );
	}
};

template <typename Self, typename F>
auto then( Self &&self, F &&fn )
{
	auto fut = into_future<typename Self::Output>( std::forward<F>( fn ) );
	return Then::then( std::forward<Self>( self ), std::move( fut ) );
}

}  // namespace _

}  // namespace utils

}  // namespace future

}  // namespace koi
