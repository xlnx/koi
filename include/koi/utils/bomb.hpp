#pragma once

#include <traits/concepts.hpp>

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;

template <typename F>
struct Bomb final : NoCopy, NoMove, NoHeap
{
	Bomb( F &&fn ) :
	  fn( std::move( fn ) ) {}

	~Bomb() { fn(); }

private:
	F fn;
};

template <typename F>
auto make_bomb( F &&fn )
{
	return Bomb( std::forward<F>( fn ) );
}

}  // namespace _

using _::make_bomb;

}  // namespace utils

}  // namespace koi
