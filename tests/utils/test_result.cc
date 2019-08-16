#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <utils/result.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace utils;

TEST( test_result, test_result )
{
	auto x = [] {
		return Result<string>::Ok( "123" );
	}();
	auto y = [] {
		return Result<string>::Err( -13 );
	}();
	ASSERT_EQ( x.ok(), "123" );
	ASSERT_EQ( y.err(), -13 );
}

enum struct Error : int
{
	WouldBlock = -1,
	Unknown = -2
};

TEST( test_result, test_result_userdefined )
{
	auto x = [] {
		return Result<string, Error>::Ok( "123" );
	}();
	auto y = [] {
		return Result<string, Error>::Err( Error::WouldBlock );
	}();
	ASSERT_EQ( x.ok(), "123" );
	ASSERT_EQ( y.err(), Error::WouldBlock );
}
