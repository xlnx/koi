#pragma once

#include "current_thread/runtime.hpp"
#include <utils/nonnull.hpp>
#include <utils/with.hpp>

namespace koi
{
namespace runtime
{
namespace _
{
using namespace koi::utils;

using DefaultRuntime = current_thread::Runtime;

inline Runtime &default_runtime()
{
	static DefaultRuntime _;
	return _;
}

template <typename F>
void run( F &&entry )
{
	if ( current_runtime().has() ) {
		current_runtime().run( std::forward<F>( entry ) );
	} else {
		current_runtime().with( default_runtime(), [&] {
			current_runtime().run( std::forward<F>( entry ) );
		} );
	}
}

template <typename F>
void spawn( F &&future )
{
	if ( current_runtime().has() ) {
		current_runtime().spawn( std::forward<F>( future ) );
	} else {
		current_runtime().with( default_runtime(), [&] {
			current_runtime().spawn( std::forward<F>( future ) );
		} );
	}
}

}  // namespace _

using _::run;
using _::spawn;
using Runtime = _::DefaultRuntime;

}  // namespace runtime

}  // namespace koi