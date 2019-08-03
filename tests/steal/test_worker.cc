#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <sync/mutex.hpp>
#include <steal/deque.hpp>
#include <steal/worker.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace sync;
using namespace koi::steal;

TEST( test_steal_worker, test_worker_lifo )
{
	Worker<int> worker( Flavor::Lifo );
	auto stealer = worker.stealer();
	worker.emplace( 1 );
	worker.emplace( 2 );
	worker.emplace( 3 );
	EXPECT_EQ( stealer.steal(), 1 );
	EXPECT_EQ( worker.pop(), 3 );
	EXPECT_EQ( worker.pop(), 2 );
}

TEST( test_steal_worker, test_worker_fifo )
{
	Worker<int> worker( Flavor::Fifo );
	auto stealer = worker.stealer();
	worker.emplace( 1 );
	worker.emplace( 2 );
	worker.emplace( 3 );
	EXPECT_EQ( stealer.steal(), 1 );
	EXPECT_EQ( worker.pop(), 2 );
	EXPECT_EQ( worker.pop(), 3 );
}
