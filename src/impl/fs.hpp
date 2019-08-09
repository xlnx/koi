#pragma once

#include <future/future.hpp>
#include <uv/evented.hpp>
#include <uv/poll.hpp>

namespace koi
{
namespace fs
{
namespace _
{
using namespace std;
using namespace future;

struct OpenFuture;

struct File
{
	static OpenFuture open( const string &path );
};

struct OpenEvent final : uv::Evented
{
	void reg( function<void( uv_loop_t * )> const &fn ) override
	{
	}
};

struct OpenFuture : Future<File>
{
private:
	OpenFuture( const string &path ) :
	  path( path ) {}

	void poll() override {}
	File poll_result() override {}

private:
	friend struct File;
	
	string path;
};

inline OpenFuture File::open( const string &path )
{
	return OpenFuture( path );
}

}  // namespace _

using _::File;
using _::OpenFuture;

}  // namespace fs

}  // namespace koi
