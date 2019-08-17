#pragma once

#include <type_traits>

#include <future/future.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>
#include <utils/result.hpp>

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

template <typename F, typename O>
struct Shared;

}  // namespace _

}  // namespace utils

namespace _
{
using namespace utils::_;
using namespace koi::utils;

template <typename Self>
struct FutureExtBasic : Self
{
	FutureExtBasic( Self &&self ) :
	  Self( std::move( self ) ) {}
};

template <typename Self, typename T = void>
struct FutureExtResultable : FutureExtBasic<Self>
{
	template <typename F>
	auto then( F &&fn ) &&;

	using FutureExtBasic<Self>::FutureExtBasic;
};

template <typename Self, typename T, typename E>
struct FutureExtResultable<Self, Result<T, E>> : FutureExtBasic<Self>
{
	// using Output = typename FutureExtBasic<Self>::Output;

	template <typename F>
	auto and_then( F &&fn ) &&;

	template <typename F>
	auto or_else( F &&fn ) &&;

	auto unwrap() &&;

	template <typename F>
	auto map_err( F &&fn ) &&;

	using FutureExtBasic<Self>::FutureExtBasic;
};

template <typename Self>
struct FutureExt : FutureExtResultable<Self, typename Self::Output>
{
	static_assert( is_base_of<Future<>, Self>::value,
				   "FutureExt must be derived from Future<>" );

	FutureExt<
	  Shared<
		FutureExt<Self>, typename Self::Output>>
	  shared() &&;

	using FutureExtResultable<Self, typename Self::Output>::FutureExtResultable;
};

// template <typename T, typename E>
// struct FutureExt<Result<T, E>> : FutureExt<>

}  // namespace _

namespace utils
{
namespace _
{
using future::_::FutureExt;
}

}  // namespace utils

}  // namespace future

}  // namespace koi
