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
	// auto inner = static_cast<Inner<T> *>(
	//   reinterpret_cast<uv_handle_t *>( _ )->data );

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

	size_t token() const { return _.token; }
	T *handle() const { return &_._; }
	bool ready() const { return _.ready; }

private:
	void reg( uv_loop_t *selector, size_t token ) const override
	{
		_.token = token;
		_.ready = false;
		fn( selector, this );
	}

public:
	static void into_poll( T *ptr ) { uv::_::into_poll<T>( ptr ); }

private:
	mutable Inner<T> _;
	F fn;
};

template <typename T, typename F>
Request<T, F> request( F &&fn )
{
	return Request<T, F>( std::forward<F>( fn ) );
}

}  // namespace _

using _::into_poll;
using _::request;

}  // namespace uv

}  // namespace koi
