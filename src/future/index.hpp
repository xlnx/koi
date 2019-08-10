#pragma once

#include "future.hpp"

#ifdef KOI_CXX_GE_20
#include "utils/c++2a/async.hpp"
#endif
#include "utils/lazy.hpp"
#include "utils/poll_fn.hpp"
#include "utils/shared.hpp"
#include "utils/then.hpp"

namespace koi
{
namespace future
{
#ifdef KOI_CXX_GE_20
using utils::Async;
#endif
using utils::lazy;
using utils::poll_fn;

}  // namespace future

#ifdef KOI_CXX_GE_20
using future::Async;
#endif

}  // namespace koi

#include "ext/future_ext_impl.hpp"
