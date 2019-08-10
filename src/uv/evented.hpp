#pragma once

#include <uv.h>

#include <traits/concepts.hpp>

namespace koi
{
namespace uv
{
namespace _
{
using namespace std;
using namespace traits::concepts;

struct Poll;

struct Evented : Dynamic
{
	virtual void reg( uv_loop_t *selector, size_t token ) const = 0;
};

}  // namespace _

using _::Evented;

}  // namespace uv

}  // namespace koi
