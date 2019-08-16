#pragma once

#include "future_ext.hpp"
#include <utils/normalize.hpp>

namespace koi
{
namespace future
{
namespace _
{
using namespace utils::_;

template <typename Self, typename F, typename R = typename NormOut<void()>::type>
struct Then
{
	static auto then( Self &&self, F &&fn )
	{
		return _::then(
		  std::forward<Self>( self ),
		  [fn = normalize( std::forward<F>( fn ) )]( R &&_ ) mutable {
			  using O = typename InvokeResultOf<decltype( fn )>::type;
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
		using R = typename NormOut<void()>::type;
		return _::then(
		  std::forward<Self>( self ),
		  [fn = normalize( std::forward<F>( fn ) )]( R &&_ ) mutable {
			  using O = typename InvokeResultOf<decltype( fn )>::type;
			  return lazy(
				[fn = std::move( fn )]() mutable -> O {
					return fn();
				} );
		  } );
	}
};

template <typename Self, typename T, typename E>
auto FutureExtResultable<Self, Result<T, E>>::unwrap() &&
{
	auto fn =
	  []( Result<T, E> res ) mutable {
		  if ( res.is_ok() ) {
			  return std::move( res.ok() );
		  } else {
			  std::abort();
		  }
	  };
	return Then<Self, decltype( fn ), Result<T, E>>::then(
	  std::move( *this ), std::move( fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::and_then( F &&fn ) &&
{
	auto and_fn =
	  [fn = normalize( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  using And = typename InvokeResultOf<decltype( fn )>::type;
		  if ( res.is_ok() ) {
			  return Result<And, E>::Ok( fn( std::move( res.ok() ) ) );
		  } else {
			  return Result<And, E>::Err( std::move( res.err() ) );
		  }
	  };
	return Then<Self, decltype( and_fn ), Result<T, E>>::then(
	  std::move( *this ), std::move( and_fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::or_else( F &&fn ) &&
{
	auto or_fn =
	  [fn = normalize( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  using Or = typename InvokeResultOf<decltype( fn )>::type;
		  static_assert( is_same<T, Or>::value,
						 "or_else() must return T" );
		  if ( res.is_ok() ) {
			  return std::move( res.ok() );
		  } else {
			  return fn( std::move( res.err() ) );
		  }
	  };
	return Then<Self, decltype( or_fn ), Result<T, E>>::then(
	  std::move( *this ), std::move( or_fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::map_err( F &&fn ) &&
{
	auto or_fn =
	  [fn = normalize( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  using X = typename InvokeResultOf<decltype( fn )>::type;
		  if ( res.is_ok() ) {
			  return Result<T, X>::Ok( std::move( res.ok() ) );
		  } else {
			  return Result<T, X>::Err( fn( std::move( res.err() ) ) );
		  }
	  };
	return Then<Self, decltype( or_fn ), Result<T, E>>::then(
	  std::move( *this ), std::move( or_fn ) );
}

template <typename Self, typename T>
template <typename F>
auto FutureExtResultable<Self, T>::then( F &&fn ) &&
{
	return Then<Self, F, typename Self::Output>::then(
	  std::move( *this ),
	  std::forward<F>( fn ) );
}

template <typename Self, typename T>
template <typename F>
auto FutureExtResultable<Self, T>::then_fut( F &&fut ) &&
{
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
