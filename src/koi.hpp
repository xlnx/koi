#pragma once

#include <future/index.hpp>
#include <runtime/runtime.hpp>

namespace koi
{
namespace runtime
{
namespace _
{
inline unique_ptr<Runtime> &_()
{
	static auto _ = unique_ptr<Runtime>( new Runtime );
	return _;
}

template <typename F>
void run( F &&entry )
{
	_()->run( std::forward<F>( entry ) );
}

template <typename F>
void spawn( F &&future )
{
	_()->spawn( std::forward<F>( future ) );
}

}  // namespace _

using _::run;
using _::spawn;

}  // namespace runtime

using runtime::run;
using runtime::spawn;

}  // namespace koi
