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

TEST( test_shared, test_shared )
{
	vector<int> a;
	auto job = future::lazy( [&] {
				   a.emplace_back( 1 );
			   } )
				 .shared();
	decltype( job ) _( job );
	koi::run( job );
	EXPECT_EQ( a, ( decltype( a ){ 1 } ) );
}
