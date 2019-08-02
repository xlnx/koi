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

using namespace co;
using namespace sync;

TEST( test_channel, test_channel_bounded )
{
	using namespace mpsc;
	auto [ tx, rx ] = channel<string>();
	// tx.send();
}