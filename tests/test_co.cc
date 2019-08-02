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
	auto e = h();
	co::spawn( e );
	co_return;
}

TEST( test_coroutine, test_co )
{
	co::run( i() );
}
