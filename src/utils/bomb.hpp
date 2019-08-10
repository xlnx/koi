#pragma once

#include <functional>

#include <traits/concepts.hpp>

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;

struct Bomb final : NoCopy, NoMove, NoHeap
{
	Bomb( function<void()> &&fn ) :
	  fn( std::move( fn ) ) {}

	~Bomb() { fn(); }

private:
	function<void()> fn;
};

}  // namespace _

using _::Bomb;

}  // namespace utils

}  // namespace koi
