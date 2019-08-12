#pragma once

#include <future/index.hpp>
#include <utils/option.hpp>
#include <uv/request.hpp>
#include <uv/poll.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace net
{
namespace _
{
using namespace std;
using namespace future;
using namespace koi::utils;
using namespace traits::concepts;

struct Inner final : NoCopy, NoMove
{
	Inner( uv_tcp_t _ ) :
	  _( _ ) {}
	~Inner()
	{
	}

	static void on_uv_connection( uv_stream_t *srv, int stat )
	{
		auto inner = reinterpret_cast<Inner *>( srv );
		inner->ready = true;
		inner->stat = stat;
	}

	uv_tcp_t _;
	bool ready = false;
	int stat = 0;
};

struct TcpStream final : NoCopy
{
	static TcpStream connect()
	{
	}

private:
};

struct TcpListener final : NoCopy
{
	static TcpListener bind( const string &host, int port )
	{
		sockaddr_in addr;
		uv_tcp_t server;
		uv_tcp_init( nullptr, &server );
		uv_ip4_addr( host.c_str(), port, &addr );
		uv_tcp_bind( &server, reinterpret_cast<const sockaddr *>( &addr ), 0 );
		return server;
	}
	auto incoming() &&
	{
		return future::utils::stream_poll_fn<int>(
		  [open = false,
		   srv = std::move( _ )]( Option<int> &_ ) mutable -> StreamState {
			  if ( !open ) {
				  auto ret = uv_listen( reinterpret_cast<uv_stream_t *>( &srv->_ ),
										128, Inner::on_uv_connection );
				  cout << uv_strerror( ret ) << endl;
				  open = true;
			  } else if ( srv->ready ) {
				  srv->ready = false;
				  _ = srv->stat;
				  return StreamState::Yield;
			  }
			  return StreamState::Pending;
		  } );
	}

private:
	TcpListener( uv_tcp_t _ ) :
	  _( Box<Inner>( new Inner( _ ) ) ) {}

private:
	Box<Inner> _;
};

}  // namespace _

using _::TcpListener;

}  // namespace net

}  // namespace koi
