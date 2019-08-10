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

## First Async Shot

```cpp
/*0123456789*/
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#include <koi.hpp>
#include <impl/fs.hpp>

using namespace std;
using namespace koi;

TEST( test_poll, test_poll )
{
	Runtime rt;
	vector<int> _;
	char buf[ 15 ] = { 0 };
	auto open_file =
	  fs::File::open( "../tests/uv/test_poll.cc" )
		.then( [&]( fs::File file ) {
			rt.spawn(
			  file.read( buf, sizeof( buf ) - 1 )
				.then( [&]( ssize_t ret ) {
					if ( ret < 0 ) {
						cerr << uv_strerror( ret ) << endl;
					} else {
						_.emplace_back( ret );
					}
					return 1;
				} )
				// .then_fut here spawns an error
				.then( [&, file]( int ) {
					rt.spawn(
					  // open with readonly mode
					  file.write( buf, 5 )
						.then( [&]( ssize_t ret ) {
							_.emplace_back( ret );
						} ) );
				} ) );
		} );
	rt.run( std::move( open_file ) );
	ASSERT_EQ( _, ( vector<int>{ 14, -9 } ) );
	ASSERT_STREQ( buf, "/*0123456789*/" );
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
- [ ] Damn
