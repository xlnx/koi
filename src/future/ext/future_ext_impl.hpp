#pragma once

#include "future_ext.hpp"
#include "prune.hpp"
#include <utils/normalize.hpp>

namespace koi
{
namespace future
{
namespace _
{
// using namespace utils::_;

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
	return _::then( std::move( *this ), std::move( fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::and_then( F &&fn ) &&
{
	auto and_fn =
	  [fn = normalize<T>( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  using And = typename InvokeResultOf<decltype( fn )>::type;
		  if ( res.is_ok() ) {
			  return Result<And, E>::Ok( fn( std::move( res.ok() ) ) );
		  } else {
			  return Result<And, E>::Err( std::move( res.err() ) );
		  }
	  };
	return _::then( std::move( *this ), std::move( and_fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::or_else( F &&fn ) &&
{
	auto or_fn =
	  [fn = normalize<E>( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  using Or = typename InvokeResultOf<decltype( fn )>::type;
		  static_assert( is_same<T, Or>::value,
						 "or_else() must return T" );
		  if ( res.is_ok() ) {
			  return std::move( res.ok() );
		  } else {
			  return fn( std::move( res.err() ) );
		  }
	  };
	return _::then( std::move( *this ), std::move( or_fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::map_err( F &&fn ) &&
{
	auto map_fn =
	  [fn = normalize<E>( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  using X = typename InvokeResultOf<decltype( fn )>::type;
		  if ( res.is_ok() ) {
			  return Result<T, X>::Ok( std::move( res.ok() ) );
		  } else {
			  return Result<T, X>::Err( fn( std::move( res.err() ) ) );
		  }
	  };
	return _::then( std::move( *this ), std::move( map_fn ) );
}

template <typename Self, typename T, typename E>
template <typename F>
auto FutureExtResultable<Self, Result<T, E>>::prune( F &&fn ) &&
{
	auto map_fn =
	  [fn = normalize<E>( std::forward<F>( fn ) )]( Result<T, E> res ) mutable {
		  if ( res.is_ok() ) {
			  return std::move( res.ok() );
		  } else {
			  fn( std::move( res.err() ) );
			  prune_current();
		  }
	  };
	return _::then( std::move( *this ), std::move( map_fn ) );
}

template <typename Self, typename T, typename E>
auto FutureExtResultable<Self, Result<T, E>>::prune() &&
{
	return std::move( *this ).prune( [] {} );
}

template <typename Self, typename T>
template <typename F>
auto FutureExtResultable<Self, T>::then( F &&fn ) &&
{
	return _::then( std::move( *this ), std::forward<F>( fn ) );
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
