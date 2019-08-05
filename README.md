# Koi

[![Build Status](https://dev.azure.com/qwq0433/qwq/_apis/build/status/xlnx.koi?branchName=master)](https://dev.azure.com/qwq0433/qwq/_build/latest?definitionId=1&branchName=master)

Koi is an async library for c++20, inspired by **tokio**. 

## Status

Under construction.

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
