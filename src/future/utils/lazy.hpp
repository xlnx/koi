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

}  // namespace _

using _::lazy;

}  // namespace utils

}  // namespace future

}  // namespace koi
