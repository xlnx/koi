#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include <version.hpp>
#ifdef KOI_CXX_GE_20
#include <experimental/coroutine>
#endif

#include <utils/normalize.hpp>
#include <utils/nonnull.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>

#define DEBUG
#include "log.hpp"

namespace koi
{
namespace runtime
{
namespace _
{
template <typename F>
void spawn( F &&future );
}

}  // namespace runtime

}  // namespace koi

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
template <typename O>
struct Shared;
}

}  // namespace utils

namespace _
{
using namespace std;
using namespace traits::concepts;
using namespace koi::utils;
#ifdef KOI_CXX_GE_20
using namespace std::experimental;
#endif

enum struct PollState : int
{
	Ok = 0,
	Pending = 1,
	Pruned = -1
};

template <typename F = void>
struct Future;

template <>
struct Future<> : Dynamic
{
	using Output = Void;

	virtual PollState poll() = 0;

	Future() = default;
	Future( Future && ) = default;
	Future &operator=( Future && ) = default;

private:
	Future( Future const & ) = default;
	Future &operator=( Future const & ) = default;

private:
	template <typename T>
	friend struct Future;
	template <typename O>
	friend struct utils::_::Shared;
};

template <typename T>
struct Future : Future<>
{
	using Output = typename NormOut<T()>::type;

	virtual T get() = 0;

	Future() = default;
	Future( Future && ) = default;
	Future &operator=( Future && ) = default;

private:
	Future( Future const & ) = default;
	Future &operator=( Future const & ) = default;
	template <typename O>
	friend struct utils::_::Shared;
};

}  // namespace _

using _::Future;
using _::PollState;

}  // namespace future

}  // namespace koi

#include "ext/future_ext.hpp"

namespace koi
{
using future::Future;
using future::PollState;
}  // namespace koi
