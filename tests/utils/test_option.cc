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
	Option<string> a;
	EXPECT_EQ( a.has_value(), false );
	a = "hello";
	EXPECT_EQ( a.has_value(), true );
	EXPECT_EQ( a.value(), "hello" );
	auto b = a;
	EXPECT_EQ( b.has_value(), true );
	EXPECT_EQ( b.value(), "hello" );
	EXPECT_EQ( a.has_value(), true );
	EXPECT_EQ( a.value(), "hello" );
	b = "world";
	a = std::move( b );
	EXPECT_EQ( a.has_value(), true );
	EXPECT_EQ( a.value(), "world" );
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
