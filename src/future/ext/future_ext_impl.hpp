#pragma once

#include "future_ext.hpp"

namespace koi
{
namespace future
{
namespace _
{
using namespace utils::_;

template <typename Self, typename F, typename R = void>
struct Then
{
	static auto then( Self &&self, F &&fn )
	{
		using O = typename InvokeResultOf<F>::type;
		return _::then(
		  std::forward<Self>( self ),
		  [fn = std::forward<F>( fn )]( R &&_ ) mutable {
			  return lazy(
				[fn = std::move( fn ),
				 _ = std::move( _ )]() mutable -> O {
					return fn( std::move( _ ) );
				} );
		  } );
	}
};

template <typename Self, typename F>
struct Then<Self, F>
{
	static auto then( Self &&self, F &&fn )
	{
		using O = typename InvokeResultOf<F>::type;
		return _::then(
		  std::forward<Self>( self ),
		  [fn = std::forward<F>( fn )]( PollOut<void> &&_ ) mutable {
			  return lazy(
				[fn = std::move( fn )]() mutable -> O {
					return fn();
				} );
		  } );
	}
};

template <typename Self>
template <typename F>
auto FutureExt<Self>::then( F &&fn ) &&
{
	return Then<Self, F, typename Self::Output>::then(
	  std::move( *this ),
	  std::forward<F>( fn ) );
}

template <typename Self>
template <typename F>
auto FutureExt<Self>::then_fut( F &&fut ) &&
{
	// using Then = typename InvokeResultOf<F>::type;
	// using Output = typename Then::Output;
	return _::then(
	  std::move( *this ),
	  std::forward<F>( fut ) );
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
