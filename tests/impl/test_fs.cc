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

TEST( test_fs, test_file_open_read_write )
{
	Runtime rt;
	vector<int> _;
	char buf[ 15 ] = { 0 };
	auto open_file =
	  fs::File::open( "../tests/impl/test_fs.cc" )
		.then( [&]( fs::File file ) {
			auto file_io =
			  file.read( buf, sizeof( buf ) - 1 )
				.then( [&]( ssize_t ret ) {
					if ( ret < 0 ) {
						cerr << uv_strerror( ret ) << endl;
					} else {
						_.emplace_back( ret );
					}
					return 1;
				} )
				.then_fut( [&, file]( int ) {
					return file.write( buf, 5 );
				} )
				.then( [&]( ssize_t ret ) {
					_.emplace_back( ret );
				} );
			// cout << sizeof( file_io ) << endl;
			rt.spawn( std::move( file_io ) );
		} );
	// cout << sizeof( open_file ) << endl;
	rt.run( std::move( open_file ) );
	ASSERT_EQ( _, ( vector<int>{ 14, -9 } ) );
	ASSERT_STREQ( buf, "/*0123456789*/" );
}
