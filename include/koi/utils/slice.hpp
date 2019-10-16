#pragma once

#include <vector>

namespace koi
{
namespace utils
{
namespace _
{
template <typename T>
struct Slice
{
	Slice( T *ptr, std::size_t len ) :
	  ptr( ptr ),
	  len( len )
	{
	}
	Slice( std::vector<T> &src ) :
	  ptr( src.data() ),
	  len( src.size() )
	{
	}

	std::size_t size() const { return len; }
	T *data() const { return ptr; }

	T *begin() const { return ptr; }
	T *end() const { return ptr + len; }

private:
	T *ptr;
	std::size_t len;
};

template <typename T>
struct ConstSlice
{
	ConstSlice( T const *ptr, std::size_t len ) :
	  ptr( ptr ),
	  len( len )
	{
	}
	ConstSlice( std::vector<T> const &src ) :
	  ptr( src.data() ),
	  len( src.size() )
	{
	}
	ConstSlice( Slice<T> const &other ) :
	  ptr( other.ptr ),
	  len( other.len )
	{
	}

	std::size_t size() const { return len; }
	T const *data() const { return ptr; }

	T const *begin() const { return ptr; }
	T const *end() const { return ptr + len; }

private:
	T const *ptr;
	std::size_t len;
};

}  // namespace _

using _::ConstSlice;
using _::Slice;

}  // namespace utils

}  // namespace koi
