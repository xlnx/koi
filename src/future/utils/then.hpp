#pragma once

#include "lazy.hpp"
#include <future/future.hpp>
#include <utils/option.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
template <typename R, typename O = void>
struct NextFnTy
{
	using type = function<R( O && )>;
	template <typename F>
	static R gen( F &a, type &fn )
	{
		return fn( a.get() );
	}
};
template <typename R>
struct NextFnTy<R>
{
	using type = function<R()>;
	template <typename F>
	static R gen( F &a, type &fn )
	{
		return fn();
	}
};

template <typename F, typename R>
struct NextFn
{
	using Ty = NextFnTy<R, typename F::Output>;
	using type = typename Ty::type;
	static R gen( F &a, type &fn ) { return Ty::gen( a, fn ); }
};

template <typename A, typename B, typename R = void>
struct Then;

template <typename A, typename B>
struct Then<A, B> : Future<>
{
	using Second = typename NextFn<A, B>::type;

	bool poll() override
	{
		while ( true )
		{
			switch ( step )
			{
			case 0:
				if ( !first.poll() ) return false;
				break;
			case 1: return second.value().poll();
			default: throw 0;
			}
			second = NextFn<A, B>::gen( first, fn );
			++step;
		}
	}

	Then( A &&a, Second &&fn ) :
	  first( std::move( a ) ),
	  fn( std::move( fn ) )
	{
	}

private:
	A first;
	Option<B> second;
	Second fn;
	int step = 0;
};

// A: Future<_>, B: Future<R>
template <typename A, typename B, typename R>
struct Then : Future<R>
{
	using Second = typename NextFn<A, B>::type;

	bool poll() override
	{
		while ( true )
		{
			switch ( step )
			{
			case 0:
				if ( !first.poll() ) return false;
				break;
			case 1: return second.value().poll();
			default: throw 0;
			}
			second = NextFn<A, B>::gen( first, fn );
			++step;
		}
	}
	R get() override { return second.value().get(); }

	Then( A &&a, Second &&fn ) :
	  first( std::move( a ) ),
	  fn( std::move( fn ) )
	{
	}

private:
	A first;
	Option<B> second;
	Second fn;
	int step = 0;
};

}  // namespace _

}  // namespace utils

}  // namespace future

}  // namespace koi
