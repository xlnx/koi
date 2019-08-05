#pragma once

#include <future/future.hpp>
#include <traits/concepts.hpp>
#include <traits/function.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
using namespace traits;

template <typename A, typename F>
struct AndThen;
}  // namespace _

}  // namespace utils

namespace _
{
using namespace utils::_;

template <typename Self>
struct Decorated final : Self
{
	static_assert( is_base_of<Future<>, Self>::value,
				   "Decorated must be derived from Future<>" );

	template <typename F>
	AndThen<Decorated, typename InvokeResultOf<F>::type>
	  and_then( F &&fn ) &&;

	Decorated( Self &&self ) :
	  Self( std::move( self ) ) {}
};

}  // namespace _

namespace utils
{
namespace _
{
using future::_::Decorated;
}

}  // namespace utils

}  // namespace future

}  // namespace koi
