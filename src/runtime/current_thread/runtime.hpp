#pragma once

#include <memory>
#include <utility>

#include <sync/queue.hpp>
#include <future/future.hpp>
#include <executor/executor.hpp>
#include <traits/concepts.hpp>

namespace koi::runtime::current_thread
{
namespace _
{
using namespace std;
using namespace traits::concepts;
using namespace future;

struct Runtime final : NoCopy
{
	struct Builder final : NoCopy
	{
		Runtime build() { return Runtime{}; }

	private:
		friend struct Runtime;
	};

	struct Executor final : executor::Executor
	{
		void spawn( unique_ptr<Future<>> &&future ) override
		{
			tasks.emplace( std::move( future ) );
		}

	private:
		sync::mpsc::Queue<unique_ptr<Future<>>> tasks;
		friend struct Runtime;
	};

	Runtime() = default;

	template <typename F>
	void run( F &&future )
	{
		this->spawn( std::move( future ) );
		while ( !this->executor.tasks.empty() )
		{
			auto front = this->executor.tasks.pop();
			front->poll();
		}
	}

	template <typename F>
	void spawn( F &&future )
	{
		this->executor.spawn( unique_ptr<F>( new F( std::move( future ) ) ) );
	}

private:
	Executor executor;
	friend struct Builder;
};

}  // namespace _

using _::Runtime;

}  // namespace koi::runtime::current_thread