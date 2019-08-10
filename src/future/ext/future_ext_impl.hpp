#pragma once

#include "future_ext.hpp"

namespace koi
{
namespace future
{
namespace _
{
using namespace utils::_;

template <typename Self>
template <typename F>
auto FutureExt<Self>::then( F &&fn ) &&
{
	using Output = typename InvokeResultOf<F>::type;
	return _::then(
	  std::move( *this ),
	  [fn = std::move( fn )]( typename Self::Output &&_ ) mutable {
		  return lazy(
			[fn = std::move( fn ),
			 _ = std::move( _ )]() mutable -> Output {
				return invoke( fn, std::move( _ ) );
			} );
	  } );
}

template <typename Self>
FutureExt<
  Shared<
	FutureExt<Self>, typename Self::Output>>
  FutureExt<Self>::shared() &&
{
	return Shared<
	  FutureExt<Self>, typename Self::Output>(
	  std::move( *this ) );
}

}  // namespace _

}  // namespace future

}  // namespace koi
