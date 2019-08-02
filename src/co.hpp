#pragma once

#include <future/future.hpp>

namespace co
{

namespace runtime
{

inline
void run(Future<> const& entry)
{
    future::DefaultExecutor::instance().run(entry);
}

inline
void spawn(Future<> const& future)
{
    future::DefaultExecutor::instance().spawn(future);
}

}

using runtime::run;
using runtime::spawn;

}
