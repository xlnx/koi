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
	static_assert( is_base_of<Future<>, B>::value,
				   "then() fn must return some future" );

	using Output = typename B::Output;

	auto first = poll_fn<B>(
	  [self = std::forward<A>( self ),
	   fn = normalize( std::forward<F>( fn ) )]( Option<B> &_ ) mutable -> bool {
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

}  // namespace _

}  // namespace utils

}  // namespace future

}  // namespace koi
