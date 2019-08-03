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

TEST( test_lazy, test_lazy )
{
	vector<int> a;
	koi::run( future::lazy( [&] {
		a.emplace_back( 1 );
		a.emplace_back( 2 );
	} ) );
	EXPECT_EQ( a, ( decltype( a ){ 1, 2 } ) );
}
