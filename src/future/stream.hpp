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

template <typename T>
struct Stream : Dynamic
{
	using Output = T;

	virtual void poll_next() = 0;
	virtual Option<T> poll_next_result() = 0;
	
};

}  // namespace _

using _::Stream;

}  // namespace future

}  // namespace koi
