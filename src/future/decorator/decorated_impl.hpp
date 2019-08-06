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

template <typename Self>
Decorated<
  Shared<
	  Decorated<Self>, typename Self::Output>>
  Decorated<Self>::shared() &&
{
	return Shared<
	  Decorated<Self>>(
	  std::move( *this ) );
}

}  // namespace _

}  // namespace future

}  // namespace koi
