#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>
#include <impl/tcp.hpp>

using namespace std;
using namespace koi;

TEST( test_tcp, test_tcp_echo_server )
{
	// Runtime rt;

	// auto listener = net::TcpListener::bind( "127.0.0.1", 5140 );
	// auto srv =
	//   std::move( listener )
	// 	.incoming()
	// 	.for_each( [&]( int x ) {
	// 		cout << x << endl;
	// 	} );

	// rt.run( std::move( srv ) );
}
