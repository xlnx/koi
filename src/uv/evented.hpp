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
using namespace utils;
using namespace traits::concepts;

struct Poll;

struct Evented : Dynamic
{
	virtual void reg( function<void( uv_loop_t * )> const &fn ) = 0;
};

}  // namespace _

using _::Evented;

}  // namespace uv

}  // namespace koi
