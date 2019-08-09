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
	FutureExt<
	  Lazy<typename InvokeResultOf<F>::type>>,
	typename InvokeResultOf<F>::type>>
  FutureExt<Self>::then( F &&fn ) &&
{
	struct Closure
	{
		Closure( F &&fn ) :
		  fn( std::move( fn ) ) {}

		FutureExt<
		  Lazy<typename InvokeResultOf<F>::type>>
		  operator()( typename Self::Output &&_ )
		{
			struct Inner
			{
				Inner( F &&fn, typename Self::Output &&_ ) :
				  fn( std::move( fn ) ),
				  _( std::move( _ ) ) {}

				typename InvokeResultOf<F>::type
				  operator()()
				{
					return fn( std::move( _ ) );
				}

				F fn;
				typename Self::Output _;
			};
			Inner inner( std::move( fn ), std::move( _ ) );
			return lazy( std::move( inner ) );
		}

		F fn;
	};
	Closure closure( std::move( fn ) );
	return Then<
	  FutureExt,
	  FutureExt<
		Lazy<typename InvokeResultOf<F>::type>>,
	  typename InvokeResultOf<F>::type>(
	  std::move( *this ),
	  std::move( closure ) );
	// std::forward<F>( fn ) );
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
