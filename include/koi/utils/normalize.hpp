#pragma once

#include <traits/function.hpp>

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits;

struct Void
{
};

template <typename R>
struct NormalizeRetImpl
{
	template <typename F>
	static auto ret( F &&fn )
	{
		return fn();
	}
};

template <>
struct NormalizeRetImpl<void>
{
	template <typename F>
	static auto ret( F &&fn )
	{
		fn();
		return Void{};
	}
};

struct NormalizeRet
{
	template <typename F>
	static auto ret( F &&fn )
	{
		return NormalizeRetImpl<typename InvokeResultOf<F>::type>::ret(
		  std::forward<F>( fn ) );
	}
};

template <typename D, typename A = tuple<>>
struct NormalizeArgsImpl
{
	template <typename F>
	static auto normalize( F &&fn )
	{
		return [fn = std::forward<F>( fn )]( D &&_ ) {
			return NormalizeRet::ret( [fn = std::move( fn )]() mutable { return fn(); } );
		};
	}
};

template <typename D, typename A>
struct NormalizeArgsImpl<D, tuple<A>>
{
	template <typename F>
	static auto normalize( F &&fn )
	{
		return [fn = std::forward<F>( fn )]( A &&_ ) {
			return NormalizeRet::ret(
			  [fn = std::move( fn ), _ = std::move( _ )]() mutable { return fn( std::move( _ ) ); } );
		};
	}
};

template <typename D, typename A1, typename... Args>
struct NormalizeArgsImpl<D, tuple<A1, Args...>>
{
	// static_assert( false, "unimplemented multi inputs" );
};

template <typename F>
struct NormOut;

template <typename F, typename D>
struct Normalize : NormalizeArgsImpl<
					 typename std::decay<D>::type,
					 typename ArgumentTypeOf<typename decay<F>::type>::type>
{
};

template <typename F>
struct Normalize<F, void> : NormalizeArgsImpl<
							  Void,
							  typename ArgumentTypeOf<typename decay<F>::type>::type>
{
};

template <typename D, typename F>
auto normalize( F &&fn )
{
	return Normalize<F, D>::normalize( std::forward<F>( fn ) );
}

template <typename F>
struct NormOut : InvokeResultOf<typename InvokeResultOf<decltype( normalize<Void, F> )>::type>
{
};

template <typename F, typename D>
struct NormIn : ArgumentTypeOf<typename InvokeResultOf<decltype( normalize<D, F> )>::type>
{
};

}  // namespace _

using _::normalize;
using _::NormIn;
using _::NormOut;
using _::Void;

}  // namespace utils

}  // namespace koi