#pragma once

#include "stream_ext.hpp"
#include <future/utils/poll_fn.hpp>

namespace koi
{
namespace future
{
namespace _
{
template <typename Self>
template <typename F>
auto StreamExt<Self>::for_each( F &&fn ) &&
{
	using Output = typename Self::Output;
	return poll_fn<void>(
	  [self = std::move( *this ),
	   fn = std::forward<F>( fn )]( PollOut<void> &_ ) mutable -> bool {
		  switch ( self.poll() ) {
		  case StreamState::Yield: fn( std::move( self.get() ) );
		  case StreamState::Pending: return false;
		  case StreamState::Done: return true;
		  default: throw 0;
		  }
	  } );
}

}  // namespace _

}  // namespace future

}  // namespace koi
