#pragma once

#include <future/index.hpp>
#include <utils/option.hpp>
#include <uv/request.hpp>
#include <uv/poll.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace fs
{
namespace _
{
using namespace std;
using namespace future;
using namespace koi::utils;
using namespace traits::concepts;

struct Inner final : NoCopy, NoMove
{
	Inner( uv_file _ ) :
	  _( _ ) {}
	~Inner()
	{
		uv_fs_close( nullptr, uv::block_request<uv_fs_t>(), _, nullptr );
	}

	uv_file _;
};

struct File
{
	static auto open( const string &path, int flags = O_RDONLY, int mode = 0 )
	{
		return uv::poll_once<File>(
		  uv::request<uv_fs_t>(
			[=]( uv_loop_t *selector, auto *request ) {
				uv_fs_open( selector, request->handle(), path.c_str(), flags, mode,
							uv::into_poll<uv_fs_t> );
			} ),
		  []( auto _ ) { return File( _->handle()->result ); } );
	}
	static auto open_sync( const string &path, int flags = O_RDONLY, int mode = 0 )
	{
		auto req = uv::block_request<uv_fs_t>();
		uv_fs_open( nullptr, req, path.c_str(), flags, mode, nullptr );
		return File( req->result );
	}

	auto read( char *buf, size_t len ) const
	{
		auto iov = uv_buf_init( buf, len );
		return uv::poll_once<ssize_t>(
		  uv::request<uv_fs_t>(
			[=, _ = _]( uv_loop_t *selector, auto *request ) {
				uv_fs_read( selector, request->handle(), _->_, &iov, 1, -1,
							uv::into_poll<uv_fs_t> );
			} ),
		  []( auto _ ) { return _->handle()->result; } );
	}
	auto read_sync( char *buf, size_t len ) const
	{
		auto iov = uv_buf_init( buf, len );
		auto req = uv::block_request<uv_fs_t>();
		uv_fs_read( nullptr, req, _->_, &iov, 1, -1, nullptr );
		return req->result;
	}

	auto write( char const *buf, size_t len ) const
	{
		auto iov = uv_buf_init( const_cast<char *>( buf ), len );
		return uv::poll_once<ssize_t>(
		  uv::request<uv_fs_t>(
			[=, _ = _]( uv_loop_t *selector, auto *request ) {
				uv_fs_write( selector, request->handle(), _->_, &iov, 1, -1,
							 uv::into_poll<uv_fs_t> );
			} ),
		  []( auto _ ) { return _->handle()->result; } );
	}
	auto write_sync( char const *buf, size_t len ) const
	{
		auto iov = uv_buf_init( const_cast<char *>( buf ), len );
		auto req = uv::block_request<uv_fs_t>();
		uv_fs_write( nullptr, req, _->_, &iov, 1, -1, nullptr );
		return req->result;
	}

	uv_file descriptor() const noexcept { return _->_; }

private:
	File( uv_file _ ) :
	  _( Arc<Inner>( new Inner( _ ) ) ) {}

private:
	Arc<Inner> _;
};

}  // namespace _

using _::File;

}  // namespace fs

}  // namespace koi
