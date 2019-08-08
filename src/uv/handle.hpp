#pragma once

#include <uv.h>
#include <utils/nonnull.hpp>

namespace koi
{
namespace uv
{
namespace _
{
template <typename T>
struct Handle;
{
	operator T *() const { return _.get(); }

	

private:
	Box<T> _ = Box<T>( new T );
};

}  // namespace _

using _::Handle;

}  // namespace uv

}  // namespace koi
