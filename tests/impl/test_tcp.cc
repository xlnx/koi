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
	vector<int> res;

	auto srv =
	  net::TcpListener::bind( "127.0.0.1", 5200 )
		.incoming()
		.take( 1 )
		.for_each( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.read( buf[ 1 ], sizeof( buf[ 1 ] ) - 1 )
				.and_then( [&, x]( ssize_t nread ) {
					res.emplace_back( nread );
					return x.write( buf[ 1 ], 5 ).unwrap();
				} ) );
		} );

	auto stream_read =
	  net::TcpStream::connect( "127.0.0.1", 5200 )
		.and_then( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.write( buf[ 0 ], sizeof( buf[ 0 ] ) - 1 )
				.and_then( [&, x] {
					return x.read( buf[ 2 ], 5 );
				} ) );
		} )
		.unwrap();

	rt.run( std::move( srv ).join( std::move( stream_read ) ) );

	ASSERT_STREQ( buf[ 0 ], "Hello World" );
	ASSERT_STREQ( buf[ 1 ], "Hello World" );
	ASSERT_STREQ( buf[ 2 ], "Hello" );
	ASSERT_EQ( res, ( vector<int>{ 30 } ) );
}

char buf[ 10 ][ 31 ] = { "Hello World" };

TEST( test_tcp, test_tcp_cxx2a )
{
#ifdef KOI_CXX_GE_20
	Runtime rt;
	vector<int> res;

	auto srv =
	  net::TcpListener::bind( "127.0.0.1", 5100 )
		.incoming()
		.take( 1 )
		.for_each( [&]( net::TcpStream x ) -> Async<> {
			auto nread = co_await x.read( buf[ 1 ], sizeof( buf[ 1 ] ) - 1 ).prune();
			co_await x.write( buf[ 1 ], 5 ).unwrap();
		} );

	auto stream_read = [&]() -> Async<> {
		auto x = co_await net::TcpStream::connect( "127.0.0.1", 5100 ).unwrap();
		co_await x.write( buf[ 0 ], sizeof( buf[ 0 ] ) - 1 ).prune();
		auto nread = co_await x.read( buf[ 2 ], 5 );
	}();

	rt.run( std::move( srv ).join( std::move( stream_read ) ) );

	ASSERT_STREQ( buf[ 0 ], "Hello World" );
	ASSERT_STREQ( buf[ 1 ], "Hello World" );
	ASSERT_STREQ( buf[ 2 ], "Hello" );
	// ASSERT_EQ( res, ( vector<int>{ 30 } ) );
#endif
}

TEST( test_tcp, test_pruning )
{
	Runtime rt;
	vector<int> res;

	auto stream_read =
	  net::TcpStream::connect( "127.0.0.1", 1 )
		.and_then( [&]( net::TcpStream x ) {
			res.emplace_back( 0 );
		} )
		.prune( [&]( uv::err::Error e ) {
			res.emplace_back( e.err_code() );
		} );

	rt.run( std::move( stream_read ) );

	ASSERT_EQ( res, ( vector<int>{ UV_ECONNREFUSED } ) );
}
