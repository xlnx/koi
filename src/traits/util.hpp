#pragma once

#include <type_traits>

namespace co::traits
{

namespace _
{

using namespace std;

// template <size_t... Args>
// struct MaxSize
// {
// };

// template <size_t... Args>
// struct MaxSize<Args...> : integral_constant<size_t, MaxSize>
// {
// };

}; // namespace _

} // namespace co::traits