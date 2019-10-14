#pragma once

#include <utility>

namespace koi
{
namespace traits
{
namespace concepts
{
namespace _
{
using namespace std;

struct NoMove
{
	NoMove() = default;

	NoMove( NoMove const & ) = default;
	NoMove &operator=( NoMove const & ) = default;
	NoMove( NoMove && ) = delete;
	NoMove &operator=( NoMove && ) = delete;
};

struct NoCopy
{
	NoCopy() = default;

	NoCopy( NoCopy && ) = default;
	NoCopy &operator=( NoCopy && ) = default;
	NoCopy( NoCopy const & ) = delete;
	NoCopy &operator=( NoCopy const & ) = delete;
};

struct ExplicitCopy
{
	ExplicitCopy() = default;

	ExplicitCopy( ExplicitCopy && ) = default;
	ExplicitCopy &operator=( ExplicitCopy && ) = default;

protected:
	explicit ExplicitCopy( ExplicitCopy const & ) = default;

private:
	ExplicitCopy &operator=( ExplicitCopy const & ) = default;
};

struct NoHeap
{
private:
	static void *operator new( size_t );
	static void *operator new[]( size_t );
};

struct Dynamic
{
	virtual ~Dynamic() = default;
};

}  // namespace _

using _::Dynamic;
using _::ExplicitCopy;
using _::NoCopy;
using _::NoHeap;
using _::NoMove;

}  // namespace concepts

}  // namespace traits

}  // namespace koi
