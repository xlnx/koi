#pragma once

#include <functional>

#include <traits/concepts.hpp>
#include "bomb.hpp"

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;

template <typename T>
struct With : NoCopy, NoHeap
{
	void with( T &_, function<void()> const &fn )
	{
		auto old = this->_;
		auto bomb = make_bomb( [this, old] { this->_ = old; } );
		this->_ = &_;
		fn();
	}

	T &operator*() const { return *_; }
	T *operator->() const { return _; }

	bool has() const { return _; }

private:
	T *_ = nullptr;
};

}  // namespace _

using _::With;

}  // namespace utils

}  // namespace koi
