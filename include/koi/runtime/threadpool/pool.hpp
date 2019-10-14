#pragma once

#include <memory>
#include <atomic>

#include <future/future.hpp>
#include <steal/deque.hpp>
#include <traits/concepts.hpp>
#include <utils/nonnull.hpp>
// #include <executor/executor.hpp>

namespace koi
{
namespace runtime
{
namespace threadpool
{
namespace _
{
using namespace std;
// using namespace executor;
using namespace steal;
using namespace traits::concepts;
using namespace utils;

enum State
{
	Idle,
	Running,
	Notified,
	Scheduled,
	Complete,
	Aborted
};

struct Task
{
	atomic<State> state;
	Box<Future<>> future;
};

struct Pool final : NoCopy, NoMove
{
	void spawn( Box<Future<>> &&future )
	{
	}

private:
	Deque<Task> global;
};

}  // namespace _

}  // namespace threadpool

}  // namespace runtime

}  // namespace koi
