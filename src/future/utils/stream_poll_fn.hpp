#pragma once

#include <future/stream.hpp>
#include <future/ext/stream_ext.hpp>
#include <utils/option.hpp>
#include <traits/function.hpp>

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
using namespace traits;

template <typename F, typename T>
struct StreamPollFn : Stream<T>
{
	StreamPollFn( F &&f ) :
	  fn( std::forward<F>( f ) ) {}

	StreamState poll() override { return this->fn( this->_ ); }
	T get() override { return std::move( _.value() ); }

private:
	F fn;
	Option<T> _;
};

template <typename T, typename F, typename = void>
future::_::StreamExt<
  StreamPollFn<F, T>>
  stream_poll_fn( F &&fn )
{
	return future::_::StreamExt<
	  StreamPollFn<F, T>>( std::forward<F>( fn ) );
}

}  // namespace _

using _::stream_poll_fn;

}  // namespace utils

}  // namespace future

}  // namespace koi
