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

using namespace std;
using namespace chrono;

using namespace koi;

TEST( test_then, test_then )
{
	vector<int> a;
	auto job = future::lazy( [&] {
				   a.emplace_back( 1 );
				   return 2;
			   } )
				 .then( [&]( int b ) {
					 a.emplace_back( b );
					 return 3;
				 } )
				 .then( [&]( int b ) {
					 a.emplace_back( b );
					 return string( "test then" );
				 } );
	ASSERT_EQ( job.poll(), PollState::Ok );
	ASSERT_EQ( job.get(), "test then" );
	ASSERT_EQ( a, ( decltype( a ){ 1, 2, 3 } ) );
}
