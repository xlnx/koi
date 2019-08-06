#pragma once

#include <iostream>
#include <memory>
#include <functional>

#include <version.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>

#ifdef KOI_CXX_GE_20
#include <experimental/coroutine>
#endif

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
#ifdef KOI_CXX_GE_20
using namespace experimental;
#endif

template <typename F = void>
struct Future;

template <>
struct Future<> : Dynamic
{
	using Output = void;

	virtual void poll() = 0;
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
	using Output = T;

	virtual T poll_result() = 0;
#ifdef KOI_CXX_GE_20
	T await_resume() noexcept
	{
		return this->poll_result();
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

#include "decorator/decorated.hpp"

namespace koi
{
using future::Future;
}
