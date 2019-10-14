#pragma once

#include <deque>

#include <traits/concepts.hpp>
#include <sync/mutex.hpp>

namespace koi
{
namespace steal
{
namespace _
{
using namespace std;
using namespace sync;
using namespace traits::concepts;

template <typename T>
struct Deque final : ExplicitCopy
{
	template <typename... Args>
	void emplace( Args &&... args )
	{
		_.lock()->emplace_back( std::forward<Args>( args )... );
	}

	T pop_front()
	{
		auto __ = _.lock();
		auto top = __->front();
		__->pop_front();
		return top;
	}
	T pop_back()
	{
		auto __ = _.lock();
		auto top = __->back();
		__->pop_back();
		return top;
	}

private:
	Mutex<deque<T>> _;
};

}  // namespace _

using _::Deque;

}  // namespace steal

}  // namespace koi