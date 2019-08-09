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

TEST( test_shared, test_shared )
{
	vector<int> a;
	auto job = future::lazy( [&] {
				   a.emplace_back( 1 );
				   return string( "shared" );
			   } )
				 .shared();
	decltype( job ) j1( job );
	decltype( job ) j2( job );
	ASSERT_EQ( j1.poll(), true );
	ASSERT_EQ( j2.get(), "shared" );
	ASSERT_EQ( a, ( decltype( a ){ 1 } ) );
}
