#pragma once

#include "future.hpp"

#ifdef KOI_CXX_GE_20
#include "utils/c++2a/async.hpp"
#endif
#include "utils/lazy.hpp"
#include "utils/then.hpp"

namespace koi
{
namespace future
{
#ifdef KOI_CXX_GE_20
using utils::Async;
#endif
using utils::lazy;

}  // namespace future

#ifdef KOI_CXX_GE_20
using future::Async;
#endif

}  // namespace koi

#include "decorator/decorated_impl.hpp"
