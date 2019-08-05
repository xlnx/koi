#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <sync/channel.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace sync;

TEST( test_channel, test_channel_bounded )
{
	using namespace mpsc;
#ifdef KOI_CXX_GE_17
	auto [ tx, rx ] = channel<string>();
#else
	auto x = channel<string>();
	auto tx = std::move( x.first );
	auto rx = std::move( x.second );
#endif
	// tx.send();
}