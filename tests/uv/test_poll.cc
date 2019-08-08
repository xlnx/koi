#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <uv/poll.hpp>

using namespace std;
using namespace chrono;

using namespace koi;

TEST( test_poll, test_poll )
{
	uv::Poll poll;
	poll.loop( [] {
		cerr << "poll()" << endl;
		return false;
	} );
}
