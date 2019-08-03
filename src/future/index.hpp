#pragma once

#include "future.hpp"

#ifdef CO_CXX_GE_20
#include "utils/c++2a/async.hpp"
#endif
#include "utils/lazy.hpp"
#include "utils/and_then.hpp"

namespace co
{
namespace future
{
#ifdef CO_CXX_GE_20
using utils::Async;
#endif
using utils::lazy;

}  // namespace future

#ifdef CO_CXX_GE_20
using future::Async;
#endif

}  // namespace co

#include "decorator/decorated_impl.hpp"
