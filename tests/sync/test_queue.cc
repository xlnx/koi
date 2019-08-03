#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <sync/queue.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace sync;

TEST( test_queue, mpsc_queue )
{
	using namespace mpsc;
	Queue<string> q;
	EXPECT_EQ( q.empty(), true );
	q.emplace( "1" );
	q.emplace( "2" );
	q.emplace( "3" );
	EXPECT_EQ( q.empty(), false );
	EXPECT_EQ( q.pop(), "1" );
	EXPECT_EQ( q.pop(), "2" );
	EXPECT_EQ( q.pop(), "3" );
	EXPECT_EQ( q.empty(), true );
}

TEST( test_queue, mpsc_queue_no_copy )
{
	using namespace mpsc;
	Queue<unique_ptr<string>> q;
	EXPECT_EQ( q.empty(), true );
	q.emplace( unique_ptr<string>( new string( "hello" ) ) );
	q.emplace( unique_ptr<string>( new string( "world" ) ) );
	decltype( q ) p;
	p = std::move( q );
	p.emplace( unique_ptr<string>( new string( "!" ) ) );
	EXPECT_EQ( p.empty(), false );
	EXPECT_EQ( *p.pop(), "hello" );
	EXPECT_EQ( *p.pop(), "world" );
	decltype( q ) r( std::move( p ) );
	EXPECT_EQ( *r.pop(), "!" );
	EXPECT_EQ( r.empty(), true );
}

TEST( test_queue, mpsc_queue_threads )
{
	using namespace mpsc;
	Queue<int> q;
	vector<thread> ths;
	for ( int i = 0; i != 4; ++i )
	{
		ths.emplace_back( [&, i] {
			q.emplace( i );
			this_thread::sleep_for( 100ms );
			q.emplace( i );
			this_thread::sleep_for( 100ms );
			q.emplace( i );
		} );
	}

	this_thread::sleep_for( 50ms );

	vector<int> res;
	for ( int i = 0; i != 2; ++i )
	{
		res.emplace_back( q.pop() );
	}

	this_thread::sleep_for( 50ms );
	for ( int i = 0; i != 2; ++i )
	{
		res.emplace_back( q.pop() );
	}

	for ( auto &th : ths )
	{
		th.join();
	}

	for ( int i = 0; i != 8; ++i )
	{
		res.emplace_back( q.pop() );
	}

	EXPECT_EQ( q.empty(), true );

	sort( res.begin() + 0, res.begin() + 4 );
	sort( res.begin() + 4, res.begin() + 8 );
	sort( res.begin() + 8, res.begin() + 12 );

	EXPECT_EQ( res, ( vector<int>{ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 } ) );
}
