#pragma once

#include "future.hpp"
#include "stream.hpp"

#ifdef KOI_CXX_GE_20
#include "utils/cxx2a/async.hpp"
#endif
#include "utils/lazy.hpp"
#include "utils/poll_fn.hpp"
#include "utils/into_future.hpp"
#include "utils/shared.hpp"
#include "utils/then.hpp"
#include "utils/join.hpp"
#include "utils/gather.hpp"
#include "utils/stream_poll_fn.hpp"

namespace koi
{
namespace future
{
#ifdef KOI_CXX_GE_20
using utils::Async;
#endif
using utils::lazy;
using utils::poll_fn;
using utils::sink;
using utils::stream_poll_fn;

}  // namespace future

#ifdef KOI_CXX_GE_20
using future::Async;
#endif

}  // namespace koi

#include "ext/future_ext_impl.hpp"
#include "ext/stream_ext_impl.hpp"
