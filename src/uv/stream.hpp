#pragma once

#include <utility>
#include <uv.h>

#include "evented.hpp"
#include "events.hpp"
#include <utils/nonnull.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace uv
{
namespace _
{
using namespace std;
using namespace utils;
using namespace traits::concepts;

template <typename T>
struct Stream : Evented, NoCopy, NoMove
{
	Stream()
	{
		_.data = this;
	}
	~Stream()
	{
	}

	virtual void on_read( ssize_t nread, const uv_buf_t *buf )
	{
	}
	virtual void on_connect( int status )
	{
	}
	virtual void on_alloc( size_t suggested, uv_buf_t *buf )
	{
	}

	T *handle() const { return &_; }
	uv_stream_t *stream_handle() const { return reinterpret_cast<uv_stream_t *>( &_ ); }

public:
	static void into_read( uv_stream_t *srv, ssize_t nread, const uv_buf_t *buf )
	{
		auto self = reinterpret_cast<Stream *>( srv->data );
		self->on_read( nread, buf );
	}
	static void into_connect( uv_stream_t *srv, int stat )
	{
		auto self = reinterpret_cast<Stream *>( srv->data );
		self->on_connect( stat );
	}
	static void into_alloc( uv_handle_t *srv, size_t suggested, uv_buf_t *buf )
	{
		auto self = reinterpret_cast<Stream *>( srv->data );
		self->on_alloc( suggested, buf );
	}

private:
	mutable T _;  // uv_stream_t
};

}  // namespace _

using _::Stream;

}  // namespace uv

}  // namespace koi
