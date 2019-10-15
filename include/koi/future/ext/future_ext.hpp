#pragma once

#include <type_traits>

#include <future/future.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>
#include <utils/result.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits;

template <typename O>
struct Shared;

struct SinkHandle;

}  // namespace _

}  // namespace utils

namespace _
{
using namespace utils::_;
using namespace koi::utils;

template <typename Self>
struct FutureExtBasic : Self
{
	FutureExtBasic( Self &&self ) :
	  Self( std::move( self ) ) {}
};

template <typename Self, typename T = void>
struct FutureExtResultable : FutureExtBasic<Self>
{
	template <typename F>
	auto then( F &&fn ) &&;

	auto gather( Arc<SinkHandle> const &h ) &&;

	template <typename... Futs>
	auto join( Futs &&... futs ) &&;

	using FutureExtBasic<Self>::FutureExtBasic;
};

template <typename Self, typename T, typename E>
struct FutureExtResultable<Self, Result<T, E>> : FutureExtBasic<Self>
{
	// using Output = typename FutureExtBasic<Self>::Output;

	template <typename F>
	auto and_then( F &&fn ) &&;

	template <typename F>
	auto or_else( F &&fn ) &&;

	auto unwrap() &&;

	template <typename F>
	auto map_err( F &&fn ) &&;

	template <typename F>
	auto prune( F &&fn ) &&;

	auto prune() &&;

	using FutureExtBasic<Self>::FutureExtBasic;
};

template <typename Self>
struct FutureExt : FutureExtResultable<Self, typename Self::Output>
{
	static_assert( is_base_of<Future<>, Self>::value,
				   "FutureExt must be derived from Future<>" );

	FutureExt<Shared<typename Self::Output>>
	  shared() &&;

	using FutureExtResultable<Self, typename Self::Output>::FutureExtResultable;
};

#ifdef KOI_CXX_GE_20
template <typename Self>
struct AwaiterImpl : NoCopy, NoMove
{
	template <typename P>
	void await_suspend( coroutine_handle<P> h ) noexcept
	{
		h.promise().set_current_future( _ );
	}
	bool await_ready() noexcept { return false; }

	AwaiterImpl( Self &&fut ) :
	  _( std::move( fut ) ) {}

protected:
	Self _;
};

template <typename Self, typename R = void>
struct TypedAwaiterImpl : AwaiterImpl<Self>
{
	R await_resume() noexcept
	{
		return this->_.get();
	}

	using AwaiterImpl<Self>::AwaiterImpl;
};

template <typename Self>
struct TypedAwaiterImpl<Self> : AwaiterImpl<Self>
{
	void await_resume() noexcept
	{
	}

	using AwaiterImpl<Self>::AwaiterImpl;
};

template <typename Self>
struct Awaiter : TypedAwaiterImpl<Self, typename Self::Output>
{
	using TypedAwaiterImpl<Self, typename Self::Output>::TypedAwaiterImpl;
};

template <typename Self>
auto operator co_await( FutureExt<Self> &&self )
{
	return Awaiter<FutureExt<Self>>( std::move( self ) );
}
#endif

}  // namespace _

namespace utils
{
namespace _
{
using future::_::FutureExt;
}

}  // namespace utils

}  // namespace future

}  // namespace koi
