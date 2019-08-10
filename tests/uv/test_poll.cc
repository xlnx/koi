#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <uv/request.hpp>
#include <uv/poll.hpp>
#include <koi.hpp>
#include <impl/fs.hpp>

using namespace std;
using namespace chrono;

using namespace koi;

TEST( test_poll, test_poll )
{
	uv::Poll poll;
	uv::Events evts;

	auto open_file = fs::File::open( "/usr/bin/bash" );
	// poll.reg( open_file, 1 );

	while ( !poll.idle() )
	{
		poll.poll( evts );
		for ( auto &evt : evts )
		{
			ASSERT_EQ( evt.token, 1 );
		}
	}
}
