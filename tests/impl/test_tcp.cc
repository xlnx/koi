#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>
#include <impl/tcp.hpp>

using namespace std;
using namespace koi;

TEST( test_tcp, test_tcp_echo_server )
{
	Runtime rt;
	char buf[ 10 ][ 31 ] = { "Hello World" };

	auto srv =
	  net::TcpListener::bind( "127.0.0.1", 5140 )
		.incoming()
		.take( 1 )
		.for_each( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.read( buf[ 1 ], sizeof( buf[ 1 ] ) - 1 )
				.then( [&, x]( ssize_t ) {
					return x.write( buf[ 1 ], 5 ).unwrap();
				} ) );
		} );

	auto stream_read =
	  net::TcpStream::connect( "127.0.0.1", 5140 )
		.and_then( [&]( net::TcpStream x ) {
			auto and_then =
			  x.write( buf[ 0 ], sizeof( buf[ 0 ] ) - 1 )
				.and_then( [&, x] {
					return x.read( buf[ 2 ], 5 );
				} );
			rt.spawn(
			  std::move( and_then ) );
		} )
		.unwrap();

	rt.spawn( std::move( srv ) );
	rt.run( std::move( stream_read ) );

	ASSERT_STREQ( buf[ 0 ], "Hello World" );
	ASSERT_STREQ( buf[ 1 ], "Hello World" );
	ASSERT_STREQ( buf[ 2 ], "Hello" );
}
