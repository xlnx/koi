#pragma once

#include <utility>
#include <uv.h>

#include "evented.hpp"
#include "events.hpp"
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

template <typename T>
struct Inner final : NoCopy
{
	T _;
	size_t token;
	bool ready = false;
};

template <typename T>
void into_poll( T *_ )
{
	auto inner = reinterpret_cast<Inner<T> *>( _ );
	// put this event into current polling Events
	// this method must be called inside a Poll::poll()
	inner->ready = true;
	Events::current()->emplace( inner->token );
}

// F: |uv_loop_t *| -> void
template <typename T, typename F>
struct Request final : Evented
{
	Request( F &&fn ) :
	  fn( std::forward<F>( fn ) ) {}

	size_t token() const { return _->token; }
	T *handle() const { return &_->_; }
	bool ready() const { return _->ready; }

private:
	void reg( uv_loop_t *selector, size_t token ) const override
	{
		_->token = token;
		_->ready = false;
		fn( selector, this );
	}

public:
	static void into_poll( T *_ ) { uv::_::into_poll<T>( _ ); }

private:
	Box<Inner<T>> _ = Box<Inner<T>>( new Inner<T> );
	F fn;
};

template <typename T, typename F>
Request<T, F> request( F &&fn )
{
	return Request<T, F>( std::forward<F>( fn ) );
}

template <typename T>
T *block_request()
{
	thread_local T _;
	return &_;
}

}  // namespace _

using _::block_request;
using _::into_poll;
using _::request;

}  // namespace uv

}  // namespace koi
