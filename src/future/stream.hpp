#pragma once

#include <future/future.hpp>
#include <utils/option.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace future
{
namespace _
{
using namespace koi::utils;

enum struct StreamState : int
{
	Pending = 0,
	Yield = 1,
	Done = -1
};

template <typename T>
struct Stream : Dynamic
{
	using Output = T;

	virtual StreamState poll() = 0;
	virtual Output get() = 0;
};

}  // namespace _

using _::Stream;
using _::StreamState;

}  // namespace future

}  // namespace koi
