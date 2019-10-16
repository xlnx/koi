#pragma once

#include <vector>
#include <new>
#include <limits>
#include <atomic>

#include <traits/concepts.hpp>

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;

using Index = size_t;

constexpr Index Null()
{
	return numeric_limits<Index>::max() - 1;
}
constexpr Index Empty()
{
	return numeric_limits<Index>::max();
}

template <typename T>
struct Cell final
{
	union
	{
		typename aligned_storage<sizeof( T ), alignof( T )>::type value;
		Index next;
	};

	Cell() :
	  next( Empty() ) {}
};

struct SlabPlaceholder
{
};

template <typename T = SlabPlaceholder>
struct Slab final : ExplicitCopy
{
	using Index = _::Index;

	Slab( size_t reserve ) :
	  _( reserve ) {}

	template <typename... Args>
	Index emplace( Args &&... args )
	{
		Index idx;
		if ( next == Null() ) {
			if ( len == _.size() ) {
				decltype( _ ) __( _.size() * 2 );
				for ( auto idx = next; idx != Null(); idx = _[ idx ].next ) {
					__[ idx ].next = _[ idx ].next;
				}
				for ( auto idx = 0; idx != _.size(); ++idx ) {
					if ( __[ idx ].next == Empty() ) {
						auto &dst = reinterpret_cast<T &>( __[ idx ].value );
						auto &src = reinterpret_cast<T &>( _[ idx ].value );
						new ( &dst ) T( std::move( src ) );
						src.~T();
					}
				}
				__.swap( _ );
			}
			idx = len++;
		} else {
			next = _[ idx = next ].next;
		}
		new ( &_[ idx ] ) T( std::forward<Args>( args )... );
		ent++;
		return idx;
	}

	T remove( Index idx )
	{
		auto &src = reinterpret_cast<T &>( _[ idx ].value );
		auto elem = std::move( src );
		src.~T();
		_[ idx ].next = next;
		next = idx;
		ent--;
		return elem;
	}

	T const &operator[]( Index idx ) const { return _[ idx ]; }
	T &operator[]( Index idx ) { return _[ idx ]; }

	std::size_t size() const { return ent.load(); }
	std::size_t capacity() const { return _.size(); }

private:
	vector<Cell<T>> _;
	Index len = 0;
	std::atomic<std::size_t> ent = 0;
	Index next = Null();
};

}  // namespace _

using _::Slab;

}  // namespace utils

}  // namespace koi
