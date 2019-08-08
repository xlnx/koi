#pragma once

#include "future_ext.hpp"

namespace koi
{
namespace future
{
namespace _
{
template <typename Self>
template <typename F>
FutureExt<
  Then<
	FutureExt<Self>,
	typename InvokeResultOf<F>::type>>
  FutureExt<Self>::then( F &&fn ) &&
{
	return Then<
	  FutureExt,
	  typename InvokeResultOf<F>::type>(
	  std::move( *this ),
	  std::forward<F>( fn ) );
}

template <typename Self>
FutureExt<
  Shared<
	  FutureExt<Self>, typename Self::Output>>
  FutureExt<Self>::shared() &&
{
	return Shared<
	  FutureExt<Self>>(
	  std::move( *this ) );
}

}  // namespace _

}  // namespace future

}  // namespace koi
