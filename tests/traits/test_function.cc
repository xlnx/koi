#include <gtest/gtest.h>

#include <tuple>
#include <functional>
#include <traits/function.hpp>

using namespace std;
using namespace koi::traits;

TEST( test_function, test_simple )
{
	auto x = []( int ) {};
	static_assert(
	  is_same<
		typename ArgumentTypeOf<decltype( x )>::type,
		tuple<int>>::value,
	  "asd" );
	static_assert(
	  is_same<
		typename InferFunction<decltype( x )>::type,
		function<void( int )>>::value,
	  "asd" );
}