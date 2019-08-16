#pragma once

#include "option.hpp"

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;

template <typename T, typename E = int>
struct Result
{
	using Item = T;
	using Error = E;

	static_assert( sizeof( E ) == sizeof( int ),
				   "result only support int-sized trivial object" );

	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
	Result( U &&value ) :
	  _( std::forward<U>( value ) )
	{
	}
	Result( Result &&other ) :
	  _( std::move( other._ ) )
	{
	}
	Result( const Result &other ) :
	  _( other )
	{
	}

	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
	Result &operator=( U &&value )
	{
		_ = std::forward<U>( value );
		return *this;
	}
	Result &operator=( Result const &other )
	{
		_ = other._;
		return *this;
	}
	Result &operator=( Result &&other )
	{
		_ = std::move( other._ );
		return *this;
	}

	bool is_ok() const { return _.has_value(); }
	bool is_err() const { return !_.has_value(); }
	explicit operator bool() const { return is_ok(); }
	T &ok() & { return _.value(); }
	const T &ok() const & { return _.value(); }
	E err() const { return reinterpret_cast<E const &>( _.state ); }

private:
	Result() = default;

public:
	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
	static Result Ok( U &&value )
	{
		return Result( std::forward<U>( value ) );
	}

	static Result Err( E const &err )
	{
		auto &err_code = reinterpret_cast<int const &>( err );
		assert( err_code != 0 );
		Result res;
		res._.state = err_code;
		return res;
	}

private:
	Option<T, char> _;
};

template <typename E>
struct Result<void, E> : Result<None, E>
{
};

}  // namespace _

using _::Result;

}  // namespace utils

}  // namespace koi