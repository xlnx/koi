# Koi

[![Build Status](https://dev.azure.com/qwq0433/qwq/_apis/build/status/xlnx.koi?branchName=master)](https://dev.azure.com/qwq0433/qwq/_build/latest?definitionId=1&branchName=master)

Koi is an c++14 async execution library based on libuv, inspired by **tokio**. 

## Status

Under construction.

## Poll Based Futures

Futures are **poll** based instead of callback based. A future does nothing unless you **poll** or **await** it on some executor.

## Future Combinators

Futures can be chained using various **combinators**.

Currently implemented combinators:

* `Future<R1>.then(Fn(R1)->R2) => Future<R2>`
* `Future<R1>.shared() => SharedFuture<R1>`

## Stream Combinators

* `Stream<R>.take(amt) => Stream<R>`
* `Stream<R>.take_while(Fn(R)->bool) => Stream<R>`
* `Stream<R>.skip(amt) => Stream<R>`
* `Stream<R>.skip_while(Fn(R)->bool) => Stream<R>`
* `Stream<R>.filter(Fn(R)->bool) => Stream<R>`
* `Stream<R>.for_each(Fn(R)) => Future<>`

## Filesystem

```cpp
/*0123456789*/
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <gtest/gtest.h>

#include <uv/request.hpp>
#include <uv/poll.hpp>
#include <koi.hpp>
#include <impl/fs.hpp>

using namespace std;
using namespace chrono;

using namespace koi;

TEST( test_fs, test_file_open_read_write )
{
	Runtime rt;
	vector<int> _;
	char buf[ 15 ] = { 0 };

	auto open_file =
	  fs::File::open( "../tests/impl/test_fs.cc" )
		.and_then( [&]( fs::File file ) {
			auto file_io =
			  file.read( buf, sizeof( buf ) - 1 )
				.and_then( [&]( ssize_t ret ) {
					_.emplace_back( ret );
				} )
				.prune()
				.then( [&, file] {
					_.emplace_back( 0 );
					return file.write( buf, 5 );
				} )
				.and_then( [&]( ssize_t ret ) {
					_.emplace_back( ret );
				} )
				.prune( [&]( uv::err::Error e ) {
					_.emplace_back( e.err_code() );
				} );

			rt.spawn( std::move( file_io ) );
		} );

	rt.run( std::move( open_file ) );

	ASSERT_EQ( _, ( vector<int>{ 14, 0, UV_EBADF } ) );
	ASSERT_STREQ( buf, "/*0123456789*/" );
}
```

## Net::Tcp

```cpp
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>
#include <impl/tcp.hpp>

using namespace std;
using namespace koi;

TEST( test_tcp, test_tcp_echo_server )
{
	Runtime rt;
	char buf[ 10 ][ 31 ] = { "Hello World" };
	vector<int> res;

	auto srv =
	  net::TcpListener::bind( "127.0.0.1", 5140 )
		.incoming()
		.take( 1 )
		.for_each( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.read( buf[ 1 ], sizeof( buf[ 1 ] ) - 1 )
				.and_then( [&, x]( ssize_t nread ) {
					res.emplace_back( nread );
					return x.write( buf[ 1 ], 5 ).unwrap();
				} ) );
		} );

	auto stream_read =
	  net::TcpStream::connect( "127.0.0.1", 5140 )
		.and_then( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.write( buf[ 0 ], sizeof( buf[ 0 ] ) - 1 )
				.and_then( [&, x] {
					return x.read( buf[ 2 ], 5 );
				} ) );
		} );

	rt.spawn( std::move( srv ) );
	rt.run( std::move( stream_read ) );

	ASSERT_STREQ( buf[ 0 ], "Hello World" );
	ASSERT_STREQ( buf[ 1 ], "Hello World" );
	ASSERT_STREQ( buf[ 2 ], "Hello" );
	ASSERT_EQ( res, ( vector<int>{ 30 } ) );
}

TEST( test_tcp, test_pruning )
{
	Runtime rt;
	vector<int> res;

	auto stream_read =
	  net::TcpStream::connect( "127.0.0.1", 1 )
		.and_then( [&]( net::TcpStream x ) {
			res.emplace_back( 0 );
		} )
		.prune( [&]( uv::err::Error e ) {
			res.emplace_back( e.err_code() );
		} );

	rt.run( std::move( stream_read ) );

	ASSERT_EQ( res, ( vector<int>{ UV_ECONNREFUSED } ) );
}
```

## Using C++20

Expected clang 5/6.

### Compile Using Apple Clang @ Mac

```bash
# with -std=c++2a -fcoroutines-ts
```

### Compile Using Clang 6.0 @ Linux

```bash
sudo dnf install libcxx-devel libcxxabi-devel  # for coroutine support
# with -std=c++2a -fcoroutines-ts -stdlib=libc++
```

## Using C++17 Or Lower

The library will be compiled without async/await support.

## Wtf

Currently not async yet:
- [X] Mutex<T>
- [X] Parker
- [X] lock free mpsc queue
- [X] mpsc channel
- [X] Slab
- [X] Stream
- [ ] Damn
