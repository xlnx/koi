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
		.and_then( [&]( fs::File file ) {
			auto file_io =
			  file.read( buf, sizeof( buf ) - 1 )
				.and_then( [&]( ssize_t ret ) {
					_.emplace_back( ret );
				} )
				.prune()
				.then( [&, file] {
					_.emplace_back( 0 );
					return file.write( buf, 5 );
				} )
				.and_then( [&]( ssize_t ret ) {
					_.emplace_back( ret );
				} )
				.prune( [&]( uv::err::Error e ) {
					_.emplace_back( e.err_code() );
				} );

			rt.spawn( std::move( file_io ) );
		} );

	rt.run( std::move( open_file ) );

	ASSERT_EQ( _, ( vector<int>{ 14, 0, UV_EBADF } ) );
	ASSERT_STREQ( buf, "/*0123456789*/" );
}
