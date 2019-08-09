#pragma once

#include <functional>
#include <vector>
#include <uv.h>

#include "events.hpp"
#include "evented.hpp"
#include <traits/concepts.hpp>
#include <utils/nonnull.hpp>

namespace koi
{
namespace uv
{
namespace _
{
using namespace std;
using namespace utils;
using namespace traits::concepts;

struct Poll final : NoCopy
{
	Poll() :
	  _( new uv_loop_t )
	{
		uv_loop_init( _.get() );
	}
	~Poll()
	{
		uv_loop_close( _.get() );
	}

	void reg( Evented &evt )
	{
		// evts.emplace_back( evts );
		// fn( _.get() );
	}
	void poll( Events &events ) const
	{
		events._.clear();
		uv_run( _.get(), UV_RUN_ONCE );
	}
	bool idle() const
	{
		return !uv_loop_alive( _.get() );
	}

	// uv_loop_t *get() const { return _.get(); }

private:
	Box<uv_loop_t> _;
	vector<NonNull<Evented>> evts;
};

}  // namespace _

using _::Poll;

}  // namespace uv

}  // namespace koi
