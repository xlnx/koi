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
		  [step = 0, evt = std::move( evt )]( Option<File> &file ) mutable -> bool {
			  switch ( step )
			  {
			  case 0:  // uv::Poll::current()->reg( *this, 0 ); break;
				  // case 1:
				  if ( !evt.ready() ) return false;
				  break;
			  case 1: return true;
			  default: throw 0;
			  }
			  ++step;
		  } );
	}
};

}  // namespace _

using _::File;

}  // namespace fs

}  // namespace koi
