#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <utils/slab.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace utils;

TEST( test_slab, test_slab )
{
	Slab<string> slab( 2 );
	auto qwert = slab.emplace( "qwert" );
	auto asd = slab.emplace( "asd" );
	auto zxcv = slab.emplace( "zxcv" );
	EXPECT_EQ( slab.remove( qwert ), "qwert" );
	auto qaz = slab.emplace( "qaz" );
	EXPECT_EQ( slab.remove( asd ), "asd" );
	auto xxxx = slab.emplace( "xxxx" );
	EXPECT_EQ( slab.remove( zxcv ), "zxcv" );
	auto www = slab.emplace( "www" );
	EXPECT_EQ( slab.remove( xxxx ), "xxxx" );
	EXPECT_EQ( slab.remove( www ), "www" );
	EXPECT_EQ( slab.remove( qaz ), "qaz" );
}
