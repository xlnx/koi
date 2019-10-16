#pragma once

#include <future/index.hpp>
#include <utils/option.hpp>
#include <utils/slice.hpp>
#include <uv/request.hpp>
#include <uv/poll.hpp>
#include <uv/errors.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>

namespace koi
{
namespace fs
{
namespace _
{
using namespace std;
using namespace future;
using namespace koi::utils;
using namespace traits;
using namespace concepts;

struct Inner final : NoCopy, NoMove
{
	Inner( uv_file _ ) :
	  _( _ )
	{
	}
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
		auto req = uv::request<uv_fs_t>(
		  [=]( uv_loop_t *selector, auto *request ) {
			  uv_fs_open( selector, request->handle(), path.c_str(), flags, mode,
						  uv::into_poll<uv_fs_t> );
		  } );
		using Ret = Result<File, uv::err::Error>;
		return uv::poll_once<Ret>(
		  std::move( req ),
		  []( decltype( req ) *_ ) {
			  if ( _->handle()->result >= 0 ) {
				  return Ret::Ok( File( _->handle()->result ) );
			  } else {
				  return Ret::Err( _->handle()->result );
			  }
		  } );
	}
	static auto open_sync( const string &path, int flags = O_RDONLY, int mode = 0 )
	{
		auto req = uv::block_request<uv_fs_t>();
		uv_fs_open( nullptr, req, path.c_str(), flags, mode, nullptr );
		return File( req->result );
	}

	auto read( ConstSlice<Slice<char>> const &slice, int64_t offset = -1 ) const
	{
		std::vector<uv_buf_t> iov;
		iov.reserve( slice.size() );
		for ( auto &s : slice ) {
			iov.emplace_back( uv_buf_init( s.data(), s.size() ) );
		}
		auto req = uv::request<uv_fs_t>(
		  [=, _ = _]( uv_loop_t *selector, auto *request ) {
			  uv_fs_read( selector, request->handle(), _->_, iov.data(), iov.size(), offset,
						  uv::into_poll<uv_fs_t> );
		  } );
		using Ret = Result<ssize_t, uv::err::Error>;
		return uv::poll_once<Ret>(
		  std::move( req ),
		  []( decltype( req ) *_ ) { if ( _->handle()->result >= 0 ) {
				  return Ret::Ok( _->handle()->result );
			  } else {
				  return Ret::Err( _->handle()->result );
			  } } );
	}
	auto read( Slice<char> const &slice, int64_t offset = -1 ) const
	{
		return read( ConstSlice<Slice<char>>( &slice, 1 ), offset );
	}
	auto read( char *buf, size_t len, int64_t offset = -1 ) const
	{
		return read( Slice<char>( buf, len ), offset );
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
		auto req = uv::request<uv_fs_t>(
		  [=, _ = _]( uv_loop_t *selector, auto *request ) {
			  uv_fs_write( selector, request->handle(), _->_, &iov, 1, -1,
						   uv::into_poll<uv_fs_t> );
		  } );
		using Ret = Result<ssize_t, uv::err::Error>;
		return uv::poll_once<Ret>(
		  std::move( req ),
		  []( decltype( req ) *_ ) {
			  if ( _->handle()->result >= 0 ) {
				  return Ret::Ok( _->handle()->result );
			  } else {
				  return Ret::Err( _->handle()->result );
			  }
		  } );
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
