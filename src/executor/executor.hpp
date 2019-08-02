#pragma once

#include <memory>

#include <future/future.hpp>
#include <traits/concepts.hpp>

namespace co::executor
{
namespace _
{
using namespace std;
using namespace future;
using namespace traits::concepts;

// template <typename F>
struct Executor : Dynamic, NoCopy
{
	virtual void spawn( unique_ptr<Future<>> &&future ) = 0;
};

}  // namespace _

using _::Executor;

}  // namespace co::executor