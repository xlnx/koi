#pragma once

#include <memory>

#include "current_thread/runtime.hpp"
#include <future/future.hpp>
#include <traits/concepts.hpp>

namespace koi::runtime
{
namespace _
{
using namespace std;
using namespace traits::concepts;

// struct Runtime final : NoCopy
// {
// 	struct Builder final : NoCopy
// 	{
// 		Builder &blocking_threads() {}

// 		Runtime build()
// 		{
// 			return Runtime( *this );
// 		}

// 	private:
// 		friend struct Runtime;
// 	};

// 	Runtime()
// 	{
// 	}

// private:
// 	Runtime( Builder const &builder )
// 	{
// 	}

// private:
// 	unique_ptr<Executor> executor;
// 	friend struct Builder;
// };

}  // namespace _

using current_thread::Runtime;

}  // namespace koi::runtime