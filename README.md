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
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>
#include <impl/fs.hpp>

using namespace std;
using namespace koi;

TEST( test_fs, test_file_open_read_write )
{
	Runtime rt;
	vector<int> _;
	char buf[ 15 ] = { 0 };
	auto open_file =
	  fs::File::open( "../tests/impl/test_fs.cc" )
		.then( [&]( fs::File file ) {
			auto file_io =
			  file.read( buf, sizeof( buf ) - 1 )
				.then( [&]( ssize_t ret ) {
					if ( ret < 0 ) {
						cerr << uv_strerror( ret ) << endl;
					} else {
						_.emplace_back( ret );
					}
					return 1;
				} )
				.then_fut( [&, file]( int ) {
					return file.write( buf, 5 );
				} )
				.then( [&]( ssize_t ret ) {
					_.emplace_back( ret );
				} );
			// cout << sizeof( file_io ) << endl;
			rt.spawn( std::move( file_io ) );
		} );
	// cout << sizeof( open_file ) << endl;
	rt.run( std::move( open_file ) );
	ASSERT_EQ( _, ( vector<int>{ 14, -9 } ) );
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

	auto srv =
	  net::TcpListener::bind( "127.0.0.1", 5140 )
		.incoming()
		.take( 1 )
		.for_each( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.read( buf[ 1 ], sizeof( buf[ 1 ] ) - 1 )
				.then_fut( [&, x]( ssize_t ) {
					return x.write( buf[ 1 ], 5 );
				} ) );
		} );

	auto stream_read =
	  net::TcpStream::connect( "127.0.0.1", 5140 )
		.then( [&]( net::TcpStream x ) {
			rt.spawn(
			  x.write( buf[ 0 ], sizeof( buf[ 0 ] ) - 1 )
				.then_fut( [&, x]( ssize_t ) {
					return x.read( buf[ 2 ], 5 );
				} ) );
		} );

	rt.spawn( std::move( srv ) );
	rt.run( std::move( stream_read ) );

	ASSERT_STREQ( buf[ 0 ], "Hello World" );
	ASSERT_STREQ( buf[ 1 ], "Hello World" );
	ASSERT_STREQ( buf[ 2 ], "Hello" );
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
