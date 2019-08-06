#pragma once

#include <type_traits>

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
using namespace std;
using namespace traits;

template <typename A, typename F>
struct Then;

template <typename F, typename O>
struct Shared;

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
	Decorated<
	  Then<
		Decorated<Self>,
		typename InvokeResultOf<F>::type>>
	  then( F &&fn ) &&;

	Decorated<
	  Shared<
		Decorated<Self>, typename Self::Output>>
	  shared() &&;

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
