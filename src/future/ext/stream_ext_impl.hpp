#pragma once

#include "stream_ext.hpp"
#include <future/utils/poll_fn.hpp>
#include <utils/normalize.hpp>

namespace koi
{
namespace future
{
namespace _
{
template <typename Self>
auto StreamExt<Self>::take( size_t amt ) &&
{
	using Output = typename Self::Output;
	return stream_poll_fn<Output>(
	  [self = std::move( *this ),
	   amt]( Option<Output> &_ ) mutable -> StreamState {
		  if ( amt == 0 ) return StreamState::Done;
		  switch ( auto state = self.poll() ) {
		  case StreamState::Yield: _ = std::move( self.get() ); --amt;
		  default: return state;
		  }
	  } );
}

template <typename Self>
template <typename F>
auto StreamExt<Self>::take_while( F &&fn ) &&
{
	using Output = typename Self::Output;
	return stream_poll_fn<Output>(
	  [self = std::move( *this ),
	   fn = normalize( std::forward<F>( fn ) )]( Option<Output> &_ ) mutable -> StreamState {
		  switch ( auto state = self.poll() ) {
		  case StreamState::Yield:
			  _ = submit( self );
			  if ( !fn( _.value() ) ) return StreamState::Done;
		  default: return state;
		  }
	  } );
}

template <typename Self>
auto StreamExt<Self>::skip( size_t amt ) &&
{
	using Output = typename Self::Output;
	return stream_poll_fn<Output>(
	  [self = std::move( *this ),
	   amt]( Option<Output> &_ ) mutable -> StreamState {
		  switch ( auto state = self.poll() ) {
		  case StreamState::Yield:
			  if ( amt > 0 ) return StreamState::Pending;
			  _ = std::move( self.get() );
			  amt -= 1;
		  default: return state;
		  }
	  } );
}

template <typename Self>
template <typename F>
auto StreamExt<Self>::skip_while( F &&fn ) &&
{
	using Output = typename Self::Output;
	return stream_poll_fn<Output>(
	  [self = std::move( *this ),
	   fn = normalize( std::forward<F>( fn ) ),
	   skipped = false]( Option<Output> &_ ) mutable -> StreamState {
		  switch ( auto state = self.poll() ) {
		  case StreamState::Yield:
			  _ = std::move( self.get() );
			  if ( !skipped ) {
				  if ( fn( _.value() ) ) return StreamState::Pending;
				  skipped = true;
			  }
		  default: return state;
		  }
	  } );
}

template <typename Self>
template <typename F>
auto StreamExt<Self>::filter( F &&fn ) &&
{
	using Output = typename Self::Output;
	return stream_poll_fn<Output>(
	  [self = std::move( *this ),
	   fn = normalize( std::forward<F>( fn ) )]( Option<Output> &_ ) mutable -> StreamState {
		  switch ( auto state = self.poll() ) {
		  case StreamState::Yield:
			  _ = std::move( self.get() );
			  if ( !fn( _.value() ) ) return StreamState::Pending;
		  default: return state;
		  }
	  } );
}

template <typename Self>
template <typename F>
auto StreamExt<Self>::for_each( F &&fn ) &&
{
	using Output = typename Self::Output;
	using Ret = typename NormOut<void()>::type;
	return poll_fn<void>(
	  [self = std::move( *this ),
	   fn = std::forward<F>( fn )]( Option<Ret> &_ ) mutable -> bool {
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
