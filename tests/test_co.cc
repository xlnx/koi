#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <queue>
#include <gtest/gtest.h>

#include <co.hpp>

#define DEBUG
#include <future/log.hpp>

using namespace std;
using namespace chrono;

using namespace co;

struct E : Executor
{
	void spawn( Future<> const &task ) override
	{
		tasks.emplace( task );
	}
	void run( Future<> const &entry ) override
	{
		spawn( entry );
		while ( !tasks.empty() )
		{
			LOG_CHECKPOINT();
			auto e = tasks.front();
			tasks.pop();
			e.poll();
		}
	}

private:
	queue<Future<>> tasks;
};

Async<string> g()
{
	LOG_CHECKPOINT();
	co_return "g";
}

Async<string> h()
{
	LOG_CHECKPOINT();
	auto x = co_await g();
	LOG_CHECKPOINT();
	EXPECT_EQ( x, "g" );
	co_return "h";
}

Async<void> i()
{
	co::spawn( h() );
	co_return;
}

TEST( test_coroutine, test_co )
{
	DefaultExecutor::set( unique_ptr<E>( new E ) );
	co::run( i() );
}
