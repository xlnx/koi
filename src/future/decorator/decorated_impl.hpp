#pragma once

#include "decorated.hpp"

namespace koi::future
{
namespace _
{
template <typename Self>
template <typename F>
AndThen<Decorated<Self>, typename InvokeResultOf<F>::type>
  Decorated<Self>::and_then( F &&fn ) &&
{
	return AndThen<
	  Decorated<Self>,
	  typename InvokeResultOf<F>::type>(
	  std::move( *this ),
	  std::move( fn ) );
}

// template <typename Self>
// template <typename F>
// AndThen<Decorated<Self>, typename InvokeResultOf<F>::type>
//   Decorated<Self>::and_then( F &&fn ) &&
// {
// 	return AndThen<
// 	  Decorated<Self>,
// 	  typename InvokeResultOf<F>::type>(
// 	  std::move( *this ),
// 	  std::move( fn ) );
// }

}  // namespace _

}  // namespace koi::future