#pragma once

namespace koi
{
namespace future
{
namespace _
{
struct PruneCurrent
{
};

inline void prune_current[ [noreturn] ]()
{
	throw PruneCurrent{};
}

}  // namespace _

using _::prune_current;
using _::PruneCurrent;

}  // namespace future

}  // namespace koi