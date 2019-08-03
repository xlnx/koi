#pragma once

#include <utility>

namespace koi::traits::concepts
{
namespace _
{
using namespace std;

struct NoMove
{
	NoMove() = default;

	NoMove( NoMove const & ) = default;
	NoMove &operator=( NoMove const & ) = default;

private:
	NoMove( NoMove && ) = default;
	NoMove &operator=( NoMove && ) = default;
};

struct NoCopy
{
	NoCopy() = default;

	NoCopy( NoCopy && ) = default;
	NoCopy &operator=( NoCopy && ) = default;

private:
	NoCopy( NoCopy const & ) = default;
	NoCopy &operator=( NoCopy const & ) = default;
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

}  // namespace koi::traits::concepts
