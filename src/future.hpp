#pragma once

#include <future>

namespace co
{

namespace _
{

using namespace std;

template <typename T>
struct Future
{
    using Output = T;
};

} // namespace _

using _::Future;

} // namespace co