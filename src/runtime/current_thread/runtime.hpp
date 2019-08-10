#pragma once

#include <memory>
#include <utility>
#include <list>

#include <reactor/reactor.hpp>
#include <sync/queue.hpp>
#include <future/future.hpp>
#include <executor/executor.hpp>
#include <traits/concepts.hpp>
#include <utils/nonnull.hpp>

namespace koi
{
namespace runtime
{
namespace current_thread
{
namespace _
{
using namespace std;
using namespace chrono;
using namespace traits::concepts;
using namespace future;
using namespace reactor;
using namespace koi::utils;
using namespace sync;

struct Runtime;

struct Inner
{
	list<Box<Future<>>> tasks;
};

struct Scheduler
{
	void schedule( Box<Future<>> &&future )
	{
		// _->tasks.emplace_back( std::move( future ) );
	}
	void tick()
	{
		auto &tasks = _->tasks;
		for ( auto itr = tasks.begin(); itr != tasks.end(); )
		{
			if ( ( *itr )->poll() )
			{
				itr = tasks.erase( itr );
			}
			else
			{
				++itr;
			}
		}
	}

	//private:
	Arc<Inner> _ = Arc<Inner>( new Inner );
};

struct Executor final : executor::Executor
{
	Executor() = default;

	void spawn( Box<Future<>> &&future ) override
	{
		scheduler._->tasks.emplace_back( std::move( future ) );
		scheduler.schedule( std::move( future ) );
	}
	void run( nanoseconds const *timeout = nullptr )
	{
		reactor.with( [=] {
			while ( true )
			{
				scheduler.tick();
				if ( reactor.idle() ) return;
				reactor.poll();
			}
		} );
	}

private:
	Scheduler scheduler;
	Reactor reactor;
	friend struct Runtime;
};

struct Runtime final : NoCopy
{
	struct Builder final : NoCopy
	{
		Runtime build() { return Runtime{}; }

	private:
		friend struct Runtime;
	};

	Runtime() = default;

	template <typename F>
	void run( F &&future )
	{
		this->spawn( std::forward<F>( future ) );
		this->executor.run();
		// while ( !this->executor.tasks.empty() )
		// {
		// 	auto front = this->executor.tasks.pop();
		// 	front->poll();
		// }
	}

	template <typename F>
	void spawn( F &&future )
	{
		using FutTy = typename decay<F>::type;
		this->executor.spawn( Box<Future<>>(
		  new FutTy( std::forward<F>( future ) ) ) );
	}

private:
	Executor executor;
	friend struct Builder;
};

}  // namespace _

using _::Runtime;

}  // namespace current_thread

}  // namespace runtime

}  // namespace koi
