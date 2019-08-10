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
	vector<int> _;
	auto open_file =
	  fs::File::open( "/usr/bin/bash" )
		.then( [&]( fs::File file ) {
			_.emplace_back( 1 );
		} );

	Runtime rt;
	rt.run( std::move( open_file ) );
	ASSERT_EQ( _, ( vector<int>{ 1 } ) );
}
