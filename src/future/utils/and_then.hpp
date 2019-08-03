#pragma once

#include <future/future.hpp>

namespace koi::future::utils
{
namespace _
{
template <typename R, typename O = void>
struct NextFnTy
{
	using type = function<R( O && )>;
	template <typename F>
	static R poll( F &a, type &fn )
	{
		return fn( a.poll_result() );
	}
};
template <typename R>
struct NextFnTy<R>
{
	using type = function<R()>;
	template <typename F>
	static R poll( F &a, type &fn )
	{
		a.poll();
		return fn();
	}
};

template <typename F, typename R = void>
struct NextFn
{
	using Ty = NextFnTy<R, typename F::Output>;
	using type = typename Ty::type;
	static R poll( F &a, type &fn ) { return Ty::poll( a, fn ); }
};

template <typename A, typename B = void>
struct AndThen;

template <typename A>
struct AndThen<A> final : Future<>
{
	using Second = typename NextFn<A>::type;

	void poll() { NextFn<A>::poll( first, fn ); }

	AndThen( A &&a, Second &&fn ) :
	  first( std::move( a ) ),
	  fn( std::move( fn ) )
	{
	}

private:
	A first;
	Second fn;
};

template <typename A, typename B>
struct AndThen final : Future<B>
{
	using Second = typename NextFn<A, B>::type;

	void poll() { _ = NextFn<A>::poll( first, fn ); }
	B poll_result() override
	{
		this->poll();
		return std::move( _ );
	}

	AndThen( A &&a, Second &&fn ) :
	  first( std::move( a ) ),
	  fn( std::move( fn ) )
	{
	}

private:
	A first;
	Second fn;
	optional<B> _;
};

}  // namespace _

}  // namespace koi::future::utils