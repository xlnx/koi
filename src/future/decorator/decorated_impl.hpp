#pragma once

#include "decorated.hpp"

namespace co::future
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

}  // namespace co::future