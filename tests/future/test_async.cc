#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <queue>
#include <gtest/gtest.h>

#include <koi.hpp>

#define DEBUG
#include <future/log.hpp>

using namespace std;
using namespace chrono;

using namespace koi;

#ifdef CO_CXX_GE_20

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
	koi::spawn( e );
	co_return;
}

#endif

TEST( test_async, test_async )
{
#ifdef CO_CXX_GE_20
	koi::run( i() );
#endif
}
