#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <utils/with.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace utils;

With<int> state;

TEST( test_with, test_with )
{
	int a = 10;
	state.with( a, [] {
		ASSERT_EQ( *state, 10 );
	} );
}
