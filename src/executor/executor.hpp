#pragma once

#include <memory>

#include <traits/concepts.hpp>
#include <future.hpp>

namespace co::executor
{

namespace _
{

using namespace std;
using namespace traits::concepts;

struct Executor : Dynamic
{
    virtual void spawn(unique_ptr<Future<void>> const &future) = 0;
};

} // namespace _

} // namespace co::executor