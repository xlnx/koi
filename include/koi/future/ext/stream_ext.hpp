#pragma once

#include <type_traits>

#include <future/future.hpp>
#include <future/stream.hpp>
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

}  // namespace _

}  // namespace utils

namespace _
{
using namespace utils::_;

template <typename Self>
struct StreamExt : Self
{
	static_assert( is_base_of<Stream<typename Self::Output>, Self>::value,
				   "StreamExt must be derived from Stream<T>" );

	auto take( size_t amt ) &&;

	template <typename F>
	auto take_while( F &&fn ) &&;

	auto skip( size_t amt ) &&;

	template <typename F>
	auto skip_while( F &&fn ) &&;

	template <typename F>
	auto filter( F &&fn ) &&;

	template <typename F>
	auto for_each( F &&fn ) &&;

	auto drain() &&;

	StreamExt( Self &&self ) :
	  Self( std::move( self ) ) {}
};

}  // namespace _

}  // namespace future

}  // namespace koi
