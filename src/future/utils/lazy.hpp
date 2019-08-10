#pragma once

#include "poll_fn.hpp"
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

// enable_if F
template <typename F>
auto lazy( F &&fn )
{
	using Output = typename InvokeResultOf<F>::type;
	return poll_fn<Output>(
	  [fn = std::forward<F>( fn )]( PollOut<Output> &_ ) mutable {
		  _ = invoke( fn );
		  return true;
	  } );
}

// template <size_t N, typename T>
// using NthTy = std::decay<decltype( std::get<N>( declval<T>() ) )>;

// template <typename F>
// auto lazy_out( F &&fn )
// {
// 	using Args = typename ArgumentTypeOf<F>::type;
// 	using Arg1Ty = typename NthTy<0, Args>::type;
// 	using Output = typename InvokeResultOf<F>::type;
// 	return poll_fn<Output>(
// 	  [fn = std::forward<F>( fn )]( PollOut<Output> &_ ) mutable {
// 		  _ = invoke( fn );
// 		  return true;
// 	  } );
// }

}  // namespace _

using _::lazy;

}  // namespace utils

}  // namespace future

}  // namespace koi
