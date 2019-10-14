#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <cassert>

#include <traits/concepts.hpp>

namespace koi
{
namespace sync
{
namespace _
{
using namespace std;
using namespace chrono;
using namespace traits::concepts;

enum UnparkerState
{
	EMPTY = 0,
	PARKED = 1,
	NOTIFIED = 2
};

struct UnparkerImpl final : NoCopy, NoMove
{
	mutable atomic<UnparkerState> state;
	mutable mutex _;
	mutable condition_variable cv;

public:
	UnparkerImpl()
	{
		state.store( EMPTY );
	}

	void park( nanoseconds const *timeout ) const
	{
		auto observed = NOTIFIED;

		if ( state.compare_exchange_strong( observed, EMPTY ) ) {
			return;
		}
		if ( timeout && timeout->count() == 0 ) {
			return;
		}

		std::unique_lock<std::mutex> _( this->_ );

		observed = EMPTY;
		if ( !state.compare_exchange_strong( observed, PARKED ) ) {
			if ( observed == NOTIFIED ) {
				auto old = state.exchange( EMPTY );
				assert( old == NOTIFIED && "park state changed unexpectedly" );
				return;
			}
			assert( false && "inconsistent park_timeout state" );
		}

		if ( timeout ) {
			cv.wait_for( _, *timeout );
			if ( state.exchange( EMPTY ) == EMPTY ) {
				assert( false && "inconsistent park_timeout state" );
			}
		} else {
			cv.wait( _, [&] {
				observed = NOTIFIED;
				return state.compare_exchange_strong( observed, EMPTY );
			} );
		}
	}

	void unpark() const
	{
		if ( state.exchange( NOTIFIED ) == PARKED ) {
			// drop(self.lock.lock().unwrap())
			cv.notify_one();
		}
	}
};

struct Parker;

struct Unparker final
{
private:
	std::shared_ptr<UnparkerImpl> _;
	friend struct Parker;

	Unparker() :
	  _( make_shared<UnparkerImpl>() )
	{
	}

public:
	void unpark() const
	{
		_->unpark();
	}
};

struct Parker final
{
private:
	Unparker _;

public:
	void park() const
	{
		_._->park( nullptr );
	}
	void park( nanoseconds const &timeout ) const
	{
		_._->park( &timeout );
	}
	const Unparker &unparker() const
	{
		return _;
	}
};

}  // namespace _

using _::Parker;
using _::Unparker;

}  // namespace sync

}  // namespace koi