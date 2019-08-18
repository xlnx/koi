#pragma once

#include "option.hpp"
#include "normalize.hpp"

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;

template <typename Self, typename T, typename E>
struct ResultImpl
{
	using Item = T;
	using Error = E;

	static_assert( sizeof( Error ) == sizeof( int ),
				   "result only support int-sized trivial object" );

	template <typename U = Item, typename = typename enable_if<
								   is_constructible<Item, U>::value>::type>
	ResultImpl( U &&value ) :
	  _( std::forward<U>( value ) )
	{
	}
	ResultImpl( ResultImpl &&other ) :
	  _( std::move( other._ ) )
	{
	}
	ResultImpl( const ResultImpl &other ) :
	  _( other._ )
	{
	}

	template <typename U = Item, typename = typename enable_if<
								   is_constructible<Item, U>::value>::type>
	ResultImpl &operator=( U &&value )
	{
		_ = std::forward<U>( value );
		return *this;
	}
	ResultImpl &operator=( ResultImpl const &other )
	{
		_ = other._;
		return *this;
	}
	ResultImpl &operator=( ResultImpl &&other )
	{
		_ = std::move( other._ );
		return *this;
	}

	bool is_ok() const { return _.has_value(); }
	bool is_err() const { return !_.has_value(); }
	explicit operator bool() const { return is_ok(); }
	Item &ok() & { return _.value(); }
	const Item &ok() const & { return _.value(); }
	Error err() const { return reinterpret_cast<Error const &>( _.state ); }

protected:
	ResultImpl() = default;

public:
	template <typename U = Item, typename = typename enable_if<
								   is_constructible<Item, U>::value>::type>
	static Self Ok( U &&value )
	{
		return Self( std::forward<U>( value ) );
	}

	static Self Err( Error const &err )
	{
		auto &err_code = reinterpret_cast<int const &>( err );
		assert( err_code != 0 );
		Self res;
		res._.state = err_code;
		return res;
	}

protected:
	Option<Item, char> _;
};

template <typename Self, typename E>
struct ResultImpl<Self, void, E> : ResultImpl<Self, Void, E>
{
	using ResultImpl<Self, Void, E>::ResultImpl;
	static Self Ok() { return Void{}; }
};

template <typename T, typename E = int>
struct Result : ResultImpl<Result<T, E>, T, E>
{
	using ResultImpl<Result<T, E>, T, E>::ResultImpl;
};

template <typename E>
struct Result<Void, E> : ResultImpl<Result<Void, E>, Void, E>
{
	using ResultImpl<Result<Void, E>, Void, E>::ResultImpl;
	template <typename U = Void, typename = typename enable_if<
								   is_constructible<Void, U>::value>::type>
	static Result Ok( U &&value )
	{
		return Void{};
	}
	static Result Ok() { return Void{}; }
};

}  // namespace _

using _::Result;

}  // namespace utils

}  // namespace koi