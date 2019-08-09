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
	ASSERT_EQ( a.has_value(), false );
	a = "hello";
	ASSERT_EQ( a.has_value(), true );
	ASSERT_EQ( a.value(), "hello" );
	auto b = a;
	ASSERT_EQ( b.has_value(), true );
	ASSERT_EQ( b.value(), "hello" );
	ASSERT_EQ( a.has_value(), true );
	ASSERT_EQ( a.value(), "hello" );
	b = "world";
	a = std::move( b );
	ASSERT_EQ( a.has_value(), true );
	ASSERT_EQ( a.value(), "world" );
}

TEST( test_option, test_option_nonnull )
{
	Option<Arc<string>> ptr = Arc<string>( New{}, "123" );
	ASSERT_EQ( ptr.has_value(), true );
	*ptr = "123";
	ASSERT_EQ( ptr.has_value(), true );
	ASSERT_EQ( *ptr, "123" );
	ptr = None{};
	ASSERT_EQ( ptr.has_value(), false );
}
