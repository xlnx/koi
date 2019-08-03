#pragma once

#include <memory>
#include <atomic>

#include <future/future.hpp>
#include <steal/deque.hpp>
#include <traits/concepts.hpp>
// #include <executor/executor.hpp>

namespace koi::runtime::threadpool
{
namespace _
{
using namespace std;
// using namespace executor;
using namespace steal;
using namespace traits::concepts;

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
	unique_ptr<Future<>> future;
};

struct Pool final : NoCopy, NoMove
{
	void spawn( unique_ptr<Future<>> &&future )
	{
	}

private:
	Deque<Task> global;
};

}  // namespace _

}  // namespace koi::runtime::threadpool