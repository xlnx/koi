#pragma once

#include <traits/concepts.hpp>
#include <executor/executor.hpp>

namespace koi::worker
{
namespace _
{
using namespace std;
using namespace executor;
using namespace traits::concepts;

struct Pool final : Executor, NoCopy
{
	void spawn( unique_ptr<Future<void>> const &future )
	{
	}
};

}  // namespace _

}  // namespace koi::worker