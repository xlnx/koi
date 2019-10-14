#pragma once

#include <queue>

#include <future/index.hpp>
#include <utils/option.hpp>
#include <utils/result.hpp>
#include <uv/request.hpp>
#include <uv/stream.hpp>
#include <uv/errors.hpp>
#include <uv/poll.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace net
{
namespace err
{
enum struct TcpConnectError : int
{
	AddrInUse = UV_EADDRINUSE,
	Unavailible = UV_EADDRNOTAVAIL
};

}  // namespace err

namespace _
{
using namespace std;
using namespace future;
using namespace koi::utils;
using namespace traits::concepts;

struct TcpListenerImpl;
struct TcpListener;
struct TcpStream;

struct TcpStreamImpl final : uv::Stream<uv_tcp_t>
{
	void open( char *buf, size_t len )
	{
		this->init();
		this->buf = buf;
		this->len = len;
		this->nread = 0;
		uv_read_start( stream_handle(), into_alloc, into_read );
	}

	bool ready() const { return len == nread; }

	void on_read( ssize_t nread, const uv_buf_t *buf ) override
	{
		if ( nread < 0 ) {
			this->err = nread;
			uv_read_stop( stream_handle() );
		} else {
			this->nread += nread;
			if ( this->nread == len ) {
				uv_read_stop( stream_handle() );
			}
		}
	}
	void on_alloc( size_t suggested, uv_buf_t *buf ) override
	{
		buf->base = this->buf;
		buf->len = this->len;
	}

	void init()
	{
		if ( !already_init ) {
			uv::Poll::current()->reg( *this, 0 );
			already_init = true;
		}
	}

private:
	void reg( uv_loop_t *selector, size_t token ) const override
	{
		uv_tcp_init( selector, this->handle() );
	}

private:
	char *buf;
	size_t len, nread;
	ssize_t err = 0;
	bool already_init = false;
	friend struct TcpStream;
};

struct TcpStream final
{
	static auto connect( const string &host, int port )
	{
		using namespace err;
		sockaddr_in addr;
		uv_ip4_addr( host.c_str(), port, &addr );
		Arc<TcpStreamImpl> impl = Arc<TcpStreamImpl>( new TcpStreamImpl );
		auto req = uv::arged_request<uv_connect_t, tuple<int>>(
		  [addr, impl]( uv_loop_t *selector, auto *request ) {
			  impl->init();
			  uv_tcp_connect(
				request->handle(),
				impl->handle(),
				reinterpret_cast<const sockaddr *>( &addr ),
				uv::into_poll<uv_connect_t, int> );
		  } );
		using Res = Result<TcpStream, uv::err::Error>;
		return uv::poll_once<Res>(
		  std::move( req ),
		  [impl]( decltype( req ) *_ ) {
			  auto stat = get<0>( _->extra() );
			  if ( stat == 0 ) {
				  return Res::Ok( TcpStream( impl ) );
			  } else {
				  return Res::Err( stat );
			  }
		  } );
	}
	auto read( char *buf, size_t len ) const
	{
		using Ret = Result<ssize_t, uv::err::Error>;
		return poll_fn<Ret>(
		  [ok = false,
		   srv = _, buf, len]( Option<Ret> &_ ) mutable -> PollState {
			  if ( !ok ) {
				  srv->open( buf, len );
				  ok = true;
			  } else if ( srv->ready() ) {
				  if ( !srv->err ) {
					  _ = Ret::Ok( srv->nread );
				  } else {
					  _ = Ret::Err( srv->err );
				  }
				  return PollState::Ok;
			  }
			  return PollState::Pending;
		  } );
	}
	auto write( char const *buf, size_t len ) const
	{
		auto iov = uv_buf_init( const_cast<char *>( buf ), len );
		auto req = uv::arged_request<uv_write_t, tuple<int>>(
		  [=, _ = _]( uv_loop_t *selector, auto *request ) {
			  uv_write( request->handle(), _->stream_handle(),
						&iov, 1, uv::into_poll<uv_write_t, int> );
		  } );
		using Res = Result<Void, uv::err::Error>;
		return uv::poll_once<Res>(
		  std::move( req ),
		  []( decltype( req ) *_ ) {
			  //   cout << get<0>( _->extra() ) << endl;
			  auto stat = get<0>( _->extra() );
			  if ( stat == 0 ) {
				  return Res::Ok();
			  } else {
				  return Res::Err( stat );
			  }
		  } );
	}

private:
	TcpStream() = default;
	TcpStream( Arc<TcpStreamImpl> const &_ ) :
	  _( _ ) {}

private:
	Arc<TcpStreamImpl> _ = Arc<TcpStreamImpl>( new TcpStreamImpl );
	friend struct TcpListenerImpl;
};

struct TcpListenerImpl final : uv::Stream<uv_tcp_t>
{
	TcpListenerImpl( sockaddr_in _ ) :
	  addr( _ ) {}

	void open()
	{
		this->init();
		uv_listen( stream_handle(), 128, into_connect );
	}

	bool ready() const { return recv.size(); }
	TcpStream take_one()
	{
		auto _ = std::move( recv.front() );
		recv.pop();
		return _;
	}

	void reg( uv_loop_t *selector, size_t token ) const override
	{
		uv_tcp_init( selector, this->handle() );
		uv_tcp_bind( handle(), reinterpret_cast<const sockaddr *>( &addr ), 0 );
	}
	void on_connect( int status ) override
	{
		stat = status;
		auto srv = this->handle();
		if ( status >= 0 ) {
			TcpStream stream;
			stream._->init();
			uv_accept( this->stream_handle(), stream._->stream_handle() );
			recv.emplace( std::move( stream ) );
		} else {
			cout << "error" << endl;
		}
	}

	void init()
	{
		if ( !already_init ) {
			uv::Poll::current()->reg( *this, 0 );
			already_init = true;
		}
	}

private:
	queue<TcpStream> recv;
	sockaddr_in addr;
	int stat = 0;
	bool already_init = false;
};

struct TcpListener final : NoCopy
{
	static TcpListener bind( const string &host, int port )
	{
		sockaddr_in addr;
		uv_ip4_addr( host.c_str(), port, &addr );
		return addr;
	}
	auto incoming() &&
	{
		return future::utils::stream_poll_fn<TcpStream>(
		  [step = 0,
		   srv = std::move( _ )]( Option<TcpStream> &_ ) mutable -> StreamState {
			  switch ( step ) {
			  case 0:
				  srv->open();
				  ++step;
			  case 1:
				  if ( srv->ready() ) {
					  _ = srv->take_one();
					  return StreamState::Yield;
				  } else {
					  return StreamState::Pending;
				  }
			  default: throw 0;
			  }
		  } );
	}

private:
	TcpListener( sockaddr_in _ ) :
	  _( Arc<TcpListenerImpl>( new TcpListenerImpl( _ ) ) ) {}

private:
	Arc<TcpListenerImpl> _;
};

}  // namespace _

using _::TcpListener;
using _::TcpStream;

}  // namespace net

}  // namespace koi
