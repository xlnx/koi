#pragma once

#include <iostream>
#include <memory>
#include <functional>

#ifdef KOI_CXX_GE_20
#include <experimental/coroutine>
#endif

#include <version.hpp>
#include <utils/normalize.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>

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
template <typename F, typename O>
struct Shared;
}

}  // namespace utils

namespace _
{
using namespace std;
using namespace traits::concepts;
using namespace koi::utils;
#ifdef KOI_CXX_GE_20
using namespace experimental;
#endif

template <typename F = void>
struct Future;

template <>
struct Future<> : Dynamic
{
	using Output = Void;

	virtual bool poll() = 0;
#ifdef KOI_CXX_GE_20
	template <typename P>
	void await_suspend( coroutine_handle<P> _ ) const noexcept
	{
		using Future = typename P::Future;
		runtime::_::spawn( Future( _ ) );
	}
	bool await_ready() const noexcept { return false; }
#endif

	Future() = default;
	Future( Future && ) = default;
	Future &operator=( Future && ) = default;

private:
	Future( Future const & ) = default;
	Future &operator=( Future const & ) = default;

private:
	template <typename T>
	friend struct Future;
	template <typename F, typename O>
	friend struct utils::_::Shared;
};

template <typename T>
struct Future : Future<>
{
	using Output = typename NormOut<T()>::type;

	virtual T get() = 0;
#ifdef KOI_CXX_GE_20
	T await_resume() noexcept
	{
		return this->get();
	}
#endif

	Future() = default;
	Future( Future && ) = default;
	Future &operator=( Future && ) = default;

private:
	Future( Future const & ) = default;
	Future &operator=( Future const & ) = default;
	template <typename F, typename O>
	friend struct utils::_::Shared;
};

}  // namespace _

using _::Future;

}  // namespace future

}  // namespace koi

#include "ext/future_ext.hpp"

namespace koi
{
using future::Future;
}
