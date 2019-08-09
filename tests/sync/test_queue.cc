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
	ASSERT_EQ( q.empty(), true );
	q.emplace( "1" );
	q.emplace( "2" );
	q.emplace( "3" );
	ASSERT_EQ( q.empty(), false );
	ASSERT_EQ( q.pop(), "1" );
	ASSERT_EQ( q.pop(), "2" );
	ASSERT_EQ( q.pop(), "3" );
	ASSERT_EQ( q.empty(), true );
}

TEST( test_queue, mpsc_queue_no_copy )
{
	using namespace mpsc;
	Queue<unique_ptr<string>> q;
	ASSERT_EQ( q.empty(), true );
	q.emplace( unique_ptr<string>( new string( "hello" ) ) );
	q.emplace( unique_ptr<string>( new string( "world" ) ) );
	decltype( q ) p;
	p = std::move( q );
	p.emplace( unique_ptr<string>( new string( "!" ) ) );
	ASSERT_EQ( p.empty(), false );
	ASSERT_EQ( *p.pop(), "hello" );
	ASSERT_EQ( *p.pop(), "world" );
	decltype( q ) r( std::move( p ) );
	ASSERT_EQ( *r.pop(), "!" );
	ASSERT_EQ( r.empty(), true );
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
			this_thread::sleep_for( milliseconds( 100 ) );
			q.emplace( i );
			this_thread::sleep_for( milliseconds( 100 ) );
			q.emplace( i );
		} );
	}

	this_thread::sleep_for( milliseconds( 50 ) );

	vector<int> res;
	for ( int i = 0; i != 2; ++i )
	{
		res.emplace_back( q.pop() );
	}

	this_thread::sleep_for( milliseconds( 50 ) );
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

	ASSERT_EQ( q.empty(), true );

	sort( res.begin() + 0, res.begin() + 4 );
	sort( res.begin() + 4, res.begin() + 8 );
	sort( res.begin() + 8, res.begin() + 12 );

	ASSERT_EQ( res, ( vector<int>{ 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3 } ) );
}
