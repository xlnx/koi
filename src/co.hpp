#pragma once

#include <future/future.hpp>
#include <runtime/runtime.hpp>

namespace co
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
	_()->run( std::move(entry) );
}

template <typename F>
void spawn( F &&future )
{
	_()->spawn( std::move(future) );
}

}  // namespace _

using _::run;
using _::spawn;

}  // namespace runtime

using runtime::run;
using runtime::spawn;

}  // namespace co
