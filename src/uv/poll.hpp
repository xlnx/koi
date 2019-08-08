#pragma once

#include <functional>
#include <uv.h>

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

struct Poll
{
	Poll() :
	  _( new uv_loop_t ),
	  hi( new uv_idle_t )
	{
		uv_loop_init( _.get() );
		uv_idle_init( _.get(), hi.get() );
		( (uv_handle_t *)hi.get() )->data = this;
	}
	~Poll()
	{
		uv_loop_close( _.get() );
	}

	void loop( function<bool()> fn )
	{
		poller().with( fn, [this] {
			uv_idle_start( hi.get(), idle );
			uv_run( _.get(), UV_RUN_DEFAULT );
		} );
	}

private:
	static With<function<bool()>> &poller()
	{
		thread_local With<function<bool()>> _;
		return _;
	}

	static void idle( uv_idle_t *hi )
	{
		auto ctx = (uv_handle_t *)hi;
		auto self = (Poll *)ctx->data;
		self->evented;
		if ( !( *poller() )() ) { uv_idle_stop( hi ); }
	}

private:
	Box<uv_idle_t> hi;
	Box<uv_loop_t> _;
	vector<void *> evented;
};

}  // namespace _

using _::Poll;

}  // namespace uv

}  // namespace koi
