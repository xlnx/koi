#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

// #include <sync/queue.hpp>
#include <future/future.hpp>

using namespace std;
using namespace chrono;

using namespace co;
using namespace future;

Future<int> f()
{
    co_return 1;
}

TEST(test_coroutine, test_co)
{
    f();
}