#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <gtest/gtest.h>

#include <executor/park.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace executor;

TEST( test_parker, park_thread )
{
	unique_ptr<UnparkThread> unpark;
	vector<int> ckpt;
	auto t1 = thread( [&] {
		ParkThread park;
		unpark.reset( new UnparkThread( park.unpark() ) );
		ckpt.emplace_back( 0 );
		park.park();
		ckpt.emplace_back( 1 );
	} );
	this_thread::sleep_for( milliseconds( 100 ) );
	ckpt.emplace_back( 2 );
	unpark->unpark();
	t1.join();
	ASSERT_EQ( ckpt, ( vector<int>{ 0, 2, 1 } ) );
}
