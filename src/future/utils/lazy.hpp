#pragma once

#include <future/future.hpp>
#include <future/decorator/decorated.hpp>

#include <traits/function.hpp>

namespace co::future::utils
{
namespace _
{
using namespace std;
using namespace traits;

using namespace util::_;

// template <typename R>
// Lazy<R> lazy( function<R()> &&fn );

template <typename R = void>
struct Lazy;

template <>
struct Lazy<> : Future<>
{
	void poll() { fn(); }

	Lazy( function<void()> &&fn ) :
	  fn( std::move( fn ) )
	{
	}

private:
	function<void()> fn;
};

template <typename R>
struct Lazy : Future<R>
{
	void poll() override { _ = fn(); }
	R poll_result() override
	{
		this->poll();
		return std::move( _ );
	}

	// private:
	Lazy( function<R()> &&fn ) :
	  fn( std::move( fn ) )
	{
	}

private:
	// template <typename F>
	// friend Lazy<F> lazy();
	function<R()> fn;
	optional<R> _;
};

template <typename F>
decltype( auto ) lazy( F &&fn )
{
	using TaskFn = typename InferFunction<F>::type;
	using R = typename InvokeResultOf<TaskFn>::type;
	return Decorated<Lazy<R>>(
	  Lazy<R>( std::move( static_cast<TaskFn &&>( fn ) ) ) );
}

}  // namespace _

using _::lazy;

}  // namespace co::future::utils