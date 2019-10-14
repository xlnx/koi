#pragma once

#include <memory>
#include <type_traits>
#include <atomic>

#include <executor/park.hpp>
#include "park.hpp"

namespace koi
{
namespace worker
{
namespace _
{
using namespace std;
using namespace executor;

struct Worker
{
	template <typename P, typename U,
			  typename = typename enable_if<
				is_base_of<Park<U>, P>::value &&
				is_base_of<Unpark, U>::value>::type>
	Worker( P &&park, U &&unpark ) :
	  park( std::forward<P>( park ) ),
	  unpark( std::forward<U>( unpark ) )
	{
		needs_drain.store( false );
	}

	ParkPtr park;
	UnparkPtr unpark;
	atomic<bool> needs_drain;
};

}  // namespace _

using _::Worker;

}  // namespace worker

}  // namespace koi