#pragma once

#include <memory>
#include <utility>

#include <traits/concepts.hpp>

namespace koi
{
namespace utils
{
namespace _
{
using namespace std;
using namespace traits::concepts;

template <typename T>
struct Option;

struct New final : NoHeap
{
};

template <typename T>
struct NonNull final
{
	NonNull( T *_ ) :
	  _( _ ) {}
	NonNull( nullptr_t ) = delete;

	template <typename... Args>
	explicit NonNull( New, Args &&... args ) :
	  _( new T( std::forward<Args>( args )... ) )
	{
	}

	T &operator*() const { return *_; }
	T *operator->() const { return _; }

private:
	T *_;
	template <typename X>
	friend struct Option;
};

template <typename T>
struct Box final
{
	explicit Box( T *_ ) :
	  _( _ ) {}
	Box( nullptr_t ) = delete;

	template <typename... Args>
	explicit Box( New, Args &&... args ) :
	  _( new T( std::forward<Args>( args )... ) )
	{
	}

	T &operator*() const { return *_; }
	T *operator->() const { return &*_; }

private:
	unique_ptr<T> _;
	template <typename X>
	friend struct Option;
};

template <typename T>
struct Arc final
{
	explicit Arc( T *_ ) :
	  _( _ ) {}
	Arc( nullptr_t ) = delete;

	template <typename... Args>
	explicit Arc( New, Args &&... args ) :
	  _( new T( std::forward<Args>( args )... ) )
	{
	}

	T &operator*() const { return *_; }
	T *operator->() const { return &*_; }

private:
	shared_ptr<T> _;
	template <typename X>
	friend struct Option;
};

}  // namespace _

using _::Arc;
using _::Box;
using _::New;
using _::NonNull;

}  // namespace utils

}  // namespace koi
