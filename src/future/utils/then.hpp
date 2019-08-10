#pragma once

#include "lazy.hpp"
#include <utils/option.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
template <typename A, typename F>
auto then( A &&self, F &&fn )
{
	using B = typename InvokeResultOf<F>::type;

	using Output = typename B::Output;

	auto first = poll_fn<B>(
	  [self = std::forward<A>( self ),
	   fn = std::forward<F>( fn )]( PollOut<B> &_ ) mutable -> bool {
		  if ( self.poll() )
		  {
			  _ = invoke( fn, self.get() );
			  return true;
		  }
		  return false;
	  } );

	return poll_fn<Output>(
	  [step = 0,
	   first = std::move( first ),
	   second = Option<B>()]( PollOut<Output> &_ ) mutable -> bool {
		  while ( true )
		  {
			  switch ( step )
			  {
			  case 0:
				  if ( !first.poll() ) return false;
				  break;
			  case 1:
				  if ( second.value().poll() )
				  {
					  _ = second.value().get();
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

}  // namespace _

}  // namespace utils

}  // namespace future

}  // namespace koi
