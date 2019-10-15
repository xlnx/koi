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

#if 0  //KOI_CXX_GE_17
template <typename T>
struct Option final
{
	Option() = default;
	Option( None ) noexcept :
	  _( nullopt ) {}

	Option( Option const &other ) :
	  _( other._ )
	{
	}
	Option( Option &&other ) :
	  _( std::move( other._ ) )
	{
	}
	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
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
	Option &operator=( Option const &other )
	{
		this->_ = other._;
		return *this;
	}
	Option &operator=( Option &&other )
	{
		this->_ = std::move( other._ );
		return *this;
	}
	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
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

template <typename Self, typename T, typename E>
struct ResultImpl;

template <typename T, typename = void>
struct Option final
{
	Option() = default;
	Option( None ) noexcept {}

	Option( const Option &other ) :
	  state( other.state )
	{
		if ( !state ) new ( reinterpret_cast<T *>( &_ ) )
		  T( reinterpret_cast<T const &>( other._ ) );
	}
	Option( Option &&other ) :
	  state( other.state )
	{
		if ( !state ) new ( reinterpret_cast<T *>( &_ ) )
		  T( std::move(
			reinterpret_cast<T &&>( other._ ) ) );
	}

	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
	Option( U &&value ) :
	  state( 0 )
	{
		new ( reinterpret_cast<T *>( &_ ) )
		  T( std::forward<U>( value ) );
	}
	template <typename... Args>
	explicit Option( New, Args &&... args ) :
	  state( 0 )
	{
		new ( reinterpret_cast<T *>( &_ ) )
		  T( std::forward<Args>( args )... );
	}
	Option &operator=( None )
	{
		if ( !state ) reinterpret_cast<T *>( &this->_ )->~T();
		state = 1;
		return *this;
	}
	template <typename U = T, typename = typename enable_if<
								is_constructible<T, U>::value>::type>
	Option &operator=( U &&value )
	{
		if ( !state ) reinterpret_cast<T *>( &this->_ )->~T();
		state = 0;
		new ( reinterpret_cast<T *>( &_ ) )
		  T( std::forward<U>( value ) );
		return *this;
	}
	Option &operator=( Option const &other )
	{
		if ( !state ) reinterpret_cast<T *>( &this->_ )->~T();
		state = other.state;
		if ( !state ) new ( reinterpret_cast<T *>( &_ ) )
		  T( reinterpret_cast<T const &>( other._ ) );
		return *this;
	}
	Option &operator=( Option &&other )
	{
		if ( !state ) reinterpret_cast<T *>( &this->_ )->~T();
		state = other.state;
		if ( !state ) new ( reinterpret_cast<T *>( &_ ) )
		  T( std::move(
			reinterpret_cast<T &&>( other._ ) ) );
		return *this;
	}
	~Option()
	{
		if ( !state ) reinterpret_cast<T *>( &_ )->~T();
	}

	bool has_value() const { return !state; }
	explicit operator bool() const { return !state; }
	T &value() & { return reinterpret_cast<T &>( _ ); }
	const T &value() const & { return reinterpret_cast<T const &>( _ ); }

private:
	typename std::aligned_storage<sizeof( T ), alignof( T )>::type _;
	int state = 1;
	template <typename Self, typename X, typename E>
	friend struct ResultImpl;
};
#endif

#define KOI_IMPL_OPTION_FOR_NONNULL_PTR( Ptr )                                     \
	template <typename T>                                                      \
	struct Option<Ptr<T>> final                                                \
	{                                                                          \
		Option() noexcept = default;                                           \
		Option( None ) noexcept : _( ( typename Ptr<T>::Pointer ) nullptr ) {} \
                                                                               \
		template <typename U = Ptr<T>>                                         \
		Option( U &&value ) : _( std::forward<U>( value ) )                    \
		{                                                                      \
		}                                                                      \
		template <typename... Args>                                            \
		explicit Option( New, Args &&... args ) :                              \
		  _( New{}, std::forward<Args>( args )... )                            \
		{                                                                      \
		}                                                                      \
		Option &operator=( None )                                              \
		{                                                                      \
			this->_._ = nullptr;                                               \
			return *this;                                                      \
		}                                                                      \
		template <typename U = Ptr<T>>                                         \
		Option &operator=( U &&value )                                         \
		{                                                                      \
			this->_ = std::forward<U>( value );                                \
			return *this;                                                      \
		}                                                                      \
                                                                               \
		typename Ptr<T>::Reference operator*() const { return *_; }            \
		typename Ptr<T>::Pointer operator->() const { return _; }              \
                                                                               \
		bool has_value() const { return bool( _._ ); }                         \
		explicit operator bool() const { return has_value(); }                 \
		Ptr<T> &value() { return _; }                                          \
                                                                               \
	private:                                                                   \
		Ptr<T> _;                                                              \
	}

KOI_IMPL_OPTION_FOR_NONNULL_PTR( NonNull );
KOI_IMPL_OPTION_FOR_NONNULL_PTR( Box );
KOI_IMPL_OPTION_FOR_NONNULL_PTR( Arc );

#undef KOI_IMPL_OPTION_FOR_NONNULL_PTR

}  // namespace _

using _::None;
using _::Option;

}  // namespace utils

}  // namespace koi
