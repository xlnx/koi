#pragma once

#include <future/future.hpp>
#include <future/ext/future_ext.hpp>
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

template <typename F, typename R = void>
struct PollFn;

inline None &none()
{
	static None _;
	return _;
}

template <typename F>
struct PollFn<F> : Future<>
{
	PollFn( F &&f ) :
	  fn( std::forward<F>( f ) ) {}

	bool poll() override { return this->fn( none() ); }

private:
	F fn;
};

template <typename F, typename R>
struct PollFn : Future<R>
{
	PollFn( F &&f ) :
	  fn( std::move( f ) ) {}

	bool poll() override { return this->fn( this->_ ); }
	R get() override { return std::move( _.value() ); }

private:
	F fn;
	Option<R> _;
};

// enable_if F
template <typename T, typename F, typename = void>
FutureExt<
  PollFn<F, T>>
  poll_fn( F &&fn )
{
	return FutureExt<
	  PollFn<F, T>>( std::forward<F>( fn ) );
}

template <typename R>
struct Invoke;

template <>
struct Invoke<void>
{
	using Output = None;
	template <typename F, typename... Args>
	static None invoke( F &&fn, Args &&... args )
	{
		std::forward<F>( fn )( std::forward<Args>( args )... );
		return None{};
	}
};

template <typename R>
struct Invoke
{
	using Output = Option<R>;
	template <typename F, typename... Args>
	static R invoke( F &&fn, Args &&... args )
	{
		return std::forward<F>( fn )( std::forward<Args>( args )... );
	}
};

template <typename F, typename... Args>
auto invoke( F &&f, Args &&... args )
{
	return Invoke<typename InvokeResultOf<F>::type>::invoke(
	  std::forward<F>( f ), std::forward<Args>( args )... );
}

template <typename T>
using PollOut = typename Invoke<T>::Output;

template <typename R>
struct Submit;

template <>
struct Submit<void>
{
	template <typename F>
	static None submit( F & )
	{
		return None{};
	}
};

template <typename R>
struct Submit
{
	template <typename F>
	static R submit( F &fut )
	{
		return fut.get();
	}
};

template <typename F>
auto submit( F &fut )
{
	return Submit<typename F::Output>::submit( fut );
}

}  // namespace _

using _::invoke;
using _::poll_fn;
using _::PollOut;

}  // namespace utils

}  // namespace future

}  // namespace koi
