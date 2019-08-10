#pragma once

#include "deque.hpp"

namespace koi
{
namespace steal
{
namespace _
{
enum Flavor
{
	Fifo = 0,
	Lifo = 1
};

template <typename T>
struct Stealer final
{
	Stealer( shared_ptr<Deque<T>> const &_, Flavor flavor ) :
	  _( _ ),
	  flavor( flavor )
	{
	}

	T steal()
	{
		return _->pop_front();
	}

private:
	shared_ptr<Deque<T>> _;
	Flavor flavor;
};

template <typename T>
struct Worker final : NoCopy
{
	using Stealer = _::Stealer<T>;

	Worker( Flavor flavor ) :
	  _( make_shared<Deque<T>>() ),
	  flavor( flavor )
	{
	}

	template <typename... Args>
	void emplace( Args &&... args )
	{
		_->emplace( std::forward<Args>( args )... );
	}

	T pop()
	{
		switch ( flavor ) {
		case Lifo:
			return _->pop_back();
		case Fifo:
			return _->pop_front();
		}
		assert( false && "unreachable" );
	}

	Stealer stealer()
	{
		return Stealer( _, flavor );
	}

private:
	shared_ptr<Deque<T>> _;
	Flavor flavor;
};

}  // namespace _

using _::Flavor;
using _::Stealer;
using _::Worker;

}  // namespace steal

}  // namespace koi