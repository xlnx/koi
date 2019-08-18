#pragma once

#include <type_traits>

#include <future/future.hpp>
#include <utils/nonnull.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;
using namespace koi::utils;

template <typename O>
struct Shared : Future<O>
{
	PollState poll() override { return _->poll(); }
	O get() override
	{
		return this->_->get();
	}

	template <typename U, typename = typename enable_if<
							is_same<typename U::Output, O>::value>::type>
	Shared( U &&_ ) :
	  _( new U( std::forward<U>( _ ) ) )
	{
	}

	Arc<Future<O>> inner() const { return _; }

private:
	Arc<Future<O>> _;
};

}  // namespace _

using _::Shared;

}  // namespace utils

}  // namespace future

}  // namespace koi
