#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <sync/mutex.hpp>

using namespace std;
using namespace chrono;

using namespace koi;
using namespace sync;

TEST( test_mutex, mutex_normal )
{
	Mutex<string> str;

	{
		auto str_lk = str.lock();
		*str_lk = "mutex";
	}

	{
		auto str_lk = str.lock();
		ASSERT_EQ( *str_lk, "mutex" );
	}
}