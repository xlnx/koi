#include <gtest/gtest.h>

#include <fn/function.hpp>

TEST( test_function, test_simple )
{
	auto x = []( int ) {};
	// co::spawn([] {

	// });
	static_assert(
	  std::is_same<
		typename co::tr::ArgumentTypeOf<decltype( x )>::type,
		std::tuple<int>>::value,
	  "asd" );
	static_assert(
	  std::is_same<
		typename co::tr::InferFunction<decltype( x )>::type,
		std::function<void( int )>>::value,
	  "asd" );
}