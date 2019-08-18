#pragma once

#include <functional>
#include <vector>
#include <uv.h>

#include "events.hpp"
#include "evented.hpp"
#include <traits/concepts.hpp>
#include <utils/nonnull.hpp>
#include <utils/with.hpp>

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

	void reg( Evented const &evt, size_t token )
	{
		evt.reg( _.get(), token );
	}
	void poll( Events &events ) const
	{
		events._.clear();
		Events::current().with( events, [&] {
			uv_run( _.get(), UV_RUN_NOWAIT );
		} );
	}
	bool idle() const
	{
		return !uv_loop_alive( _.get() );
	}

	uv_loop_t *get() const { return _.get(); }

public:
	static With<Poll> &current()
	{
		thread_local With<Poll> _;
		return _;
	}

private:
	Box<uv_loop_t> _;
};

}  // namespace _

using _::Poll;

}  // namespace uv

}  // namespace koi
