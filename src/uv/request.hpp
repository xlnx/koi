#pragma once

#include <utility>
#include <uv.h>

#include "evented.hpp"
#include "events.hpp"
#include "poll.hpp"
#include <future/index.hpp>
#include <utils/nonnull.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace uv
{
namespace _
{
using namespace std;
using namespace utils;
using namespace traits::concepts;

template <typename T, typename A>
struct Inner final : NoCopy
{
	T _;
	size_t token;
	bool ready = false;
	A extra;
};

template <typename T, typename... Args>
void into_poll( T *_, Args... args )
{
	auto inner = reinterpret_cast<Inner<T, tuple<Args...>> *>( _ );
	// put this event into current polling Events
	// this method must be called inside a Poll::poll()
	inner->ready = true;
	inner->extra = make_tuple( args... );
	Events::current()->emplace( inner->token );
}

// F: |uv_loop_t *| -> void
template <typename T, typename F, typename A = tuple<>>
struct Request final : Evented
{
	Request( F &&fn ) :
	  fn( std::forward<F>( fn ) ) {}

	size_t token() const { return _->token; }
	T *handle() const { return &_->_; }
	bool ready() const { return _->ready; }
	A const &extra() const { return _->extra; }

private:
	void reg( uv_loop_t *selector, size_t token ) const override
	{
		_->token = token;
		_->ready = false;
		fn( selector, this );
	}

private:
	Box<Inner<T, A>> _ = Box<Inner<T, A>>( new Inner<T, A> );
	F fn;
};

template <typename T, typename F>
Request<T, F> request( F &&fn )
{
	return Request<T, F>( std::forward<F>( fn ) );
}

template <typename T, typename A, typename F>
Request<T, F, A> arged_request( F &&fn )
{
	return Request<T, F, A>( std::forward<F>( fn ) );
}

template <typename T>
T *block_request()
{
	thread_local T _;
	return &_;
}

template <typename T, typename R, typename F>
auto poll_once( R &&req, F &&fn )
{
	using O = typename NormOut<T()>::type;
	return future::poll_fn<O>(
	  [ok = false,
	   fn = normalize<R>( std::forward<F>( fn ) ),
	   req = std::forward<R>( req )]( Option<O> &_ ) mutable -> PollState {
		  if ( !ok ) {
			  uv::Poll::current()->reg( req, 0 );
			  ok = true;
		  } else if ( req.ready() ) {
			  _ = fn( &req );
			  return PollState::Ok;
		  }
		  return PollState::Pending;
	  } );
}

}  // namespace _

using _::arged_request;
using _::block_request;
using _::into_poll;
using _::poll_once;
using _::request;

}  // namespace uv

}  // namespace koi
