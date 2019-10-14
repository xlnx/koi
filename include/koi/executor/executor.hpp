#pragma once

#include <memory>

#include <future/future.hpp>
#include <traits/concepts.hpp>
#include <utils/nonnull.hpp>

namespace koi
{
namespace executor
{
namespace _
{
using namespace std;
using namespace future;
using namespace traits::concepts;
using namespace koi::utils;

// template <typename F>
struct Executor : Dynamic, NoCopy
{
	virtual void spawn( Box<Future<>> &&future ) = 0;
};

}  // namespace _

using _::Executor;

}  // namespace executor

}  // namespace koi
