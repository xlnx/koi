/*0123456789*/
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
	Runtime rt;
	vector<int> _;
	char buf[ 15 ] = { 0 };
	auto open_file =
	  fs::File::open( "../tests/uv/test_poll.cc" )
		.then( [&]( fs::File file ) {
			rt.spawn(
			  file.read( buf, sizeof( buf ) - 1 )
				.then( [&]( ssize_t ret ) {
					if ( ret < 0 ) {
						cerr << uv_strerror( ret ) << endl;
					} else {
						_.emplace_back( ret );
					}
					return 1;
				} )
				// .then_fut here spawns an error
				.then( [&, file]( int ) {
					rt.spawn(
					  // open with readonly mode
					  file.write( buf, 5 )
						.then( [&]( ssize_t ret ) {
							_.emplace_back( ret );
						} ) );
				} ) );
		} );
	rt.run( std::move( open_file ) );
	ASSERT_EQ( _, ( vector<int>{ 14, -9 } ) );
	ASSERT_STREQ( buf, "/*0123456789*/" );
}
