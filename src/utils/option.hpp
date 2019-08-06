#pragma once

#include <version.hpp>

#ifdef KOI_CXX_GE_17
#include <optional>
#else
#include <new>
#endif

#include "nonnull.hpp"
#include <traits/concepts.hpp>

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;

struct None
{
};

#ifdef KOI_CXX_GE_17
template <typename T>
struct Option final
{
	Option() = default;
	Option( None ) noexcept :
	  _( nullopt ) {}

	template <typename U = T>
	Option( U &&value ) :
	  _( std::forward<U>( value ) )
	{
	}
	template <typename... Args>
	explicit Option( New, Args &&... args ) :
	  _( std::in_place, std::forward<Args>( args )... )
	{
	}
	Option &operator=( None )
	{
		this->_ = std::in_place;
		return *this;
	}
	template <typename U = T>
	Option &operator=( U &&value )
	{
		this->_ = std::forward<U>( value );
		return *this;
	}

	bool has_value() const { return _.has_value(); }
	explicit operator bool() const { return has_value(); }
	T &value() & { return _.value(); }
	const T &value() const & { return _.value(); }

private:
	optional<T> _;
};
#else
template <typename T>
struct Option final
{
	Option() = default;
	Option( None ) noexcept {}

	Option( const Option &other ) :
	  has( other.has )
	{
		if ( has ) new ( reinterpret_cast<T *>( &_ ) )
		  T( reinterpret_cast<T const &>( other._ ) );
	}
	Option( Option &&other ) :
	  has( other.has )
	{
		if ( has ) new ( reinterpret_cast<T *>( &_ ) )
		  T( std::move(
			reinterpret_cast<T &&>( other._ ) ) );
	}

	template <typename U = T, typename = typename enable_if<is_constructible<T, U>::value>::type>
	Option( U &&value ) :
	  has( true )
	{
		new ( reinterpret_cast<T *>( &_ ) )
		  T( std::forward<U>( value ) );
	}
	template <typename... Args>
	explicit Option( New, Args &&... args ) :
	  has( true )
	{
		new ( reinterpret_cast<T *>( &_ ) )
		  T( std::forward<Args>( args )... );
	}
	Option &operator=( None )
	{
		if ( has ) reinterpret_cast<T *>( &this->_ )->~T();
		has = false;
		return *this;
	}
	template <typename U = T, typename = typename enable_if<is_constructible<T, U>::value>::type>
	Option &operator=( U &&value )
	{
		if ( has ) reinterpret_cast<T *>( &this->_ )->~T();
		has = true;
		new ( reinterpret_cast<T *>( &_ ) )
		  T( std::forward<U>( value ) );
		return *this;
	}
	Option &operator=( Option const &other )
	{
		if ( has ) reinterpret_cast<T *>( &this->_ )->~T();
		has = other.has;
		if ( has ) new ( reinterpret_cast<T *>( &_ ) )
		  T( reinterpret_cast<T const &>( other._ ) );
		return *this;
	}
	Option &operator=( Option &&other )
	{
		if ( has ) reinterpret_cast<T *>( &this->_ )->~T();
		has = other.has;
		if ( has ) new ( reinterpret_cast<T *>( &_ ) )
		  T( std::move(
			reinterpret_cast<T &&>( other._ ) ) );
		return *this;
	}
	~Option()
	{
		if ( has ) reinterpret_cast<T *>( &_ )->~T();
	}

	bool has_value() const { return has; }
	explicit operator bool() const { return has; }
	T &value() & { return reinterpret_cast<T &>( _ ); }
	const T &value() const & { return reinterpret_cast<T const &>( _ ); }

private:
	typename std::aligned_storage<sizeof( T ), alignof( T )>::type _;
	bool has = false;
};
#endif

#define IMPL_OPTION_FOR_NONNULL_PTR( Ptr )                     \
	template <typename T>                                      \
	struct Option<Ptr<T>> final                                \
	{                                                          \
		Option() noexcept = default;                           \
		Option( None ) noexcept : _( (T *)nullptr ) {}         \
                                                               \
		template <typename U = Ptr<T>>                         \
		Option( U &&value ) : _( std::forward<U>( value ) )    \
		{                                                      \
		}                                                      \
		template <typename... Args>                            \
		explicit Option( New, Args &&... args ) :              \
		  _( New{}, std::forward<Args>( args )... )            \
		{                                                      \
		}                                                      \
		Option &operator=( None )                              \
		{                                                      \
			this->_ = nullptr;                                 \
			return *this;                                      \
		}                                                      \
		template <typename U = Ptr<T>>                         \
		Option &operator=( U &&value )                         \
		{                                                      \
			this->_ = std::forward<U>( value );                \
			return *this;                                      \
		}                                                      \
                                                               \
		T &operator*() const { return *_; }                    \
		T *operator->() const { return _; }                    \
                                                               \
		bool has_value() const { return bool( _._ ); }         \
		explicit operator bool() const { return has_value(); } \
		Ptr<T> value() const { return _; }                     \
                                                               \
	private:                                                   \
		Ptr<T> _;                                              \
	}

IMPL_OPTION_FOR_NONNULL_PTR( NonNull );
IMPL_OPTION_FOR_NONNULL_PTR( Box );
IMPL_OPTION_FOR_NONNULL_PTR( Arc );

}  // namespace _

using _::None;
using _::Option;

}  // namespace utils

}  // namespace koi
