#pragma once

#include <future/index.hpp>
#include <utils/option.hpp>
#include <uv/request.hpp>
#include <uv/poll.hpp>

namespace koi
{
namespace fs
{
namespace _
{
using namespace std;
using namespace future;
using namespace koi::utils;

struct File
{
	static auto open( const string &path, int flags = 1, int mode = -1 )
	{
		auto evt = uv::request<uv_fs_t>(
		  [=]( uv_loop_t *selector, auto *request ) {
			  uv_fs_open( selector, request->handle(), path.c_str(), flags, mode,
						  uv::into_poll<uv_fs_t> );
		  } );
		return poll_fn<File>(
		  [ok = false,
		   evt = std::move( evt )]( Option<File> &_ ) mutable -> bool {
			  if ( !ok ) {
				  uv::Poll::current()->reg( evt, 0 );
				  ok = true;
			  } else if ( evt.ready() ) {
				  _ = File{};
				  return true;
			  }
			  return false;
		  } );
	}
	auto read()
	{
		auto evt = uv::request<uv_fs_t>(
		  [=]( uv_loop_t *selector, auto *request ) {
			  uv_fs_read( selector, request->handle(), _, bufs, nbufs, off,
						  uv::into_poll<uv_fs_t> );
		  } );
		return poll_fn<void>(
		  [ok = false,
		   evt = std::move( evt )]( None &_ ) mutable -> bool {
			  if ( !ok ) {
				  uv::Poll::current()->reg( evt, 0 );
				  ok = true;
			  } else {
			  }
			  return false;
		  } );
	}

private:
	uv_file _;
};

}  // namespace _

using _::File;

}  // namespace fs

}  // namespace koi
