#pragma once

#include <memory>
#include <utility>
#include <thread>

#include "pool.hpp"
#include <future/future.hpp>
#include <executor/executor.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace runtime
{
namespace threadpool
{
namespace _
{
using namespace std;
using namespace traits::concepts;
using namespace future;

struct Runtime final NoCopy
{
	struct Builder final : NoCopy
	{
		Runtime build()
		{
			return Runtime( *this );
		}

	private:
		size_t core_threads = thread::hardware_concurrency();
		friend struct Runtime;
	};

	struct Executor final : executor::Executor
	{
		void spawn( unique_ptr<Future<>> &&future ) override
		{
			pool->spawn( std::move( future ) );
		}

	private:
		friend struct Runtime;
		Box<Pool> pool;
	};

	template <typename F>
	void run( F &&future )
	{
		this->spawn( std::forward<F>( future ) );
		while ( !this->executor.tasks.empty() ) {
			//
		}
	}

	template <typename F>
	void spawn( F &&future )
	{
		using FutTy = typename decay<F>::type;
		this->executor.spawn( Box<FutTy>(
		  new FutTy( std::forward<F>( future ) ) ) );
	}

private:
	Runtime( const Builder &builder )
	{
	}

private:
	Executor executor;
	friend struct Builder;
};

}  // namespace _

}  // namespace threadpool

}  // namespace runtime

}  // namespace koi
