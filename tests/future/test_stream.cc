#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>

using namespace std;

using namespace koi;
using namespace future;

thread_local int poll_cnt = 0;

struct BoundedIntStream : Stream<int>
{
	BoundedIntStream()
	{
		for ( auto x : { 1, 2, 3, 4, 5 } ) {
			_.emplace( x );
		}
	}

	StreamState poll() override
	{
		// 1 0 1 0 1 0 1 0 1 0 -1
		++poll_cnt;
		if ( ( flip = !flip ) ) {
			return _.empty() ? StreamState::Done : StreamState::Yield;
		} else {
			return StreamState::Pending;
		}
	}
	int get() override
	{
		auto x = _.front();
		_.pop();
		return x;
	}

private:
	queue<int> _;
	bool flip = false;
};

TEST( test_stream, test_stream )
{
	vector<int> v;
	_::StreamExt<BoundedIntStream> is = BoundedIntStream();
	koi::run(
	  std::move( is )
		.for_each( [&]( int x ) {
			v.emplace_back( x );
		} ) );
	ASSERT_EQ( v, ( vector<int>{ 1, 2, 3, 4, 5 } ) );
	ASSERT_EQ( poll_cnt, 11 );
}
