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

namespace
{
struct EmptyTy
{
};

}  // namespace

template <typename F, typename R = void>
struct Normalize
{
	static auto normalize( F &&fn ) { return std::move( fn ); }
};

template <typename F, typename A>
struct NormalizeImpl;

template <typename F, typename... Args>
struct NormalizeImpl<F, tuple<Args...>>
{
	static auto normalize( F &&fn )
	{
		return [fn = std::move( fn )]( Args... args ) {
			fn( args... );
			return EmptyTy{};
		};
	}
};

template <typename F>
struct Normalize<F>
{
	static auto normalize( F &&fn )
	{
		using A = typename ArgumentTypeOf<typename decay<F>::type>::type;
		return NormalizeImpl<F, A>::normalize( std::move( fn ) );
	}
};

template <typename F>
auto normalize( F &&fn )
{
	using O = typename InvokeResultOf<F>::type;
	return Normalize<F, O>::normalize( std::move( fn ) );
}

template <typename F>
struct NormOut : InvokeResultOf<typename InvokeResultOf<decltype( normalize<F> )>::type>
{
};

}  // namespace _

using _::normalize;
using _::NormOut;

}  // namespace utils

}  // namespace koi