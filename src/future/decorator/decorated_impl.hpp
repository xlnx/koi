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
  Then<
	Decorated<Self>,
	typename InvokeResultOf<F>::type>>
  Decorated<Self>::then( F &&fn ) &&
{
	return Then<
	  Decorated,
	  typename InvokeResultOf<F>::type>(
	  std::move( *this ),
	  std::forward<F>( fn ) );
}

}  // namespace _

}  // namespace future

}  // namespace koi
