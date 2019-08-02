#pragma once

#include <memory>
#include <future/future.hpp>

namespace co::runtime
{
namespace _
{
using namespace std;

struct Runtime final
{
	struct Builder final
	{
		Builder &blocking_threads() {}

		Runtime build()
		{
			return Runtime( *this );
		}

	private:
		friend struct Runtime;
	};

	Runtime()
	{
	}

private:
	Runtime( Builder const &builder )
	{
	}

private:
	unique_ptr<Executor> executor;
	friend struct Builder;
};

}  // namespace _

}  // namespace co::runtime