#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>

#define DEBUG
#include <future/log.hpp>

using namespace std;
using namespace chrono;

using namespace koi;

TEST( test_then, test_then )
{
	vector<int> a;
	auto job = future::lazy( [&] {
				   a.emplace_back( 1 );
				   a.emplace_back( 2 );
				   return 3;
			   } )
				 .then( [&]( int b ) {
					 a.emplace_back( 3 );
				 } );
	koi::run( std::move( job ) );
	EXPECT_EQ( a, ( decltype( a ){ 1, 2, 3 } ) );
}
