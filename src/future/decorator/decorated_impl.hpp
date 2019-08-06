#pragma once

#include "decorated.hpp"

namespace koi
{
namespace future
{
namespace _
{
template <typename Self>
template <typename F>
Decorated<
  AndThen<
	Decorated<Self>,
	typename InvokeResultOf<F>::type>>
  Decorated<Self>::and_then( F &&fn ) &&
{
	return AndThen<
	  Decorated,
	  typename InvokeResultOf<F>::type>(
	  std::move( *this ),
	  std::forward<F>( fn ) );
}

}  // namespace _

}  // namespace future

}  // namespace koi
