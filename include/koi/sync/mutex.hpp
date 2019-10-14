#pragma once

#include <mutex>
#include <utility>

#include <traits/concepts.hpp>

namespace koi
{
namespace sync
{
namespace _
{
using namespace std;
using namespace traits::concepts;

template <typename T>
struct Mutex;

template <typename T>
struct MutexGuard final : NoCopy, NoHeap
{
	MutexGuard( MutexGuard &&other ) :
	  _( other._ )
	{
		other._ = nullptr;
	}
	MutexGuard &operator=( MutexGuard &&other )
	{
		_ = other._;
		other._ = nullptr;
		return *this;
	}
	~MutexGuard()
	{
		if ( _ ) {
			_->lk.unlock();
		}
	}

	T &operator*() const
	{
		return _->_;
	}
	T *operator->() const
	{
		return &_->_;
	}

private:
	MutexGuard( Mutex<T> const &_ ) :
	  _( &_ )
	{
		_.lk.lock();
	}

private:
	Mutex<T> const *_;
	template <typename U>
	friend struct Mutex;
};

template <typename T>
struct Mutex final
{
	template <typename... Args>
	Mutex( Args &&... args ) :
	  _( std::forward<Args>( args )... )
	{
	}

	MutexGuard<T> lock() const
	{
		return MutexGuard<T>( *this );
	}

private:
	mutable mutex lk;
	mutable T _;
	template <typename U>
	friend struct MutexGuard;
};

}  // namespace _

using _::Mutex;

}  // namespace sync

}  // namespace koi
