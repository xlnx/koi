#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <utils/option.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace utils;

TEST( test_option, test_option )
{
	Option<int> a;
	EXPECT_EQ( a.has_value(), false );
	a = 2;
	EXPECT_EQ( a.has_value(), true );
	EXPECT_EQ( a.value(), 2 );
}

TEST( test_option, test_option_nonnull )
{
	Option<Arc<string>> ptr = Arc<string>( New{}, "123" );
	EXPECT_EQ( ptr.has_value(), true );
	*ptr = "123";
	EXPECT_EQ( ptr.has_value(), true );
	EXPECT_EQ( *ptr, "123" );
	ptr = None{};
	EXPECT_EQ( ptr.has_value(), false );
}
