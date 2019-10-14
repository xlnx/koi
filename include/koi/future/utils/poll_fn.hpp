#pragma once

#include <future/future.hpp>
#include <future/ext/future_ext.hpp>
#include <utils/option.hpp>
#include <utils/normalize.hpp>
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

template <typename F, typename R = void>
struct PollFn;

inline auto &none()
{
	using O = Void;
	static Option<O> _;
	return _;
}

template <typename F>
struct PollFn<F> : Future<>
{
	PollFn( F &&f ) :
	  fn( std::forward<F>( f ) ) {}

	PollState poll() override { return this->fn( none() ); }

private:
	F fn;
};

template <typename F, typename R>
struct PollFn : Future<R>
{
	PollFn( F &&f ) :
	  fn( std::forward<F>( f ) ) {}

	PollState poll() override { return this->fn( this->_ ); }
	R get() override { return std::move( _.value() ); }

private:
	F fn;
	Option<R> _;
};

// enable_if F
template <typename T, typename F, typename = void>
FutureExt<
  PollFn<F, typename NormOut<T()>::type>>
  poll_fn( F &&fn )
{
	using O = typename NormOut<T()>::type;
	return FutureExt<
	  PollFn<F, O>>( std::forward<F>( fn ) );
}

}  // namespace _

// using _::invoke;
using _::poll_fn;
// using _::PollOut;

}  // namespace utils

}  // namespace future

}  // namespace koi
