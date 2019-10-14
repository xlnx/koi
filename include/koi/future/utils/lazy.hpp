#pragma once

#include "poll_fn.hpp"
#include <traits/function.hpp>
#include <utils/normalize.hpp>

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
	using Output = typename NormOut<F>::type;
	return poll_fn<Output>(
	  [fn = normalize<Void>( std::forward<F>( fn ) )]( Option<Output> &_ ) mutable {
		  _ = fn( Void{} );
		  return PollState::Ok;
	  } );
}

}  // namespace _

using _::lazy;

}  // namespace utils

}  // namespace future

}  // namespace koi
