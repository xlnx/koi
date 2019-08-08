#pragma once

#include <future/future.hpp>

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

struct OpenFuture : Future<File>
{
private:
	OpenFuture( const string &path ) :
	  path( path ) {}

	void poll() override {  }

private:
	friend struct File;
	string path;
};

inline OpenFuture File::open( const string &path )
{
	return OpenFuture( path );
}

}  // namespace _

}  // namespace fs

}  // namespace koi
