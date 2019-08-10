#pragma once

#include <uv/poll.hpp>
#include <utils/nonnull.hpp>
#include <utils/slab.hpp>
#include <future/future.hpp>
#include <sync/queue.hpp>
#include <executor/park.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace reactor
{
namespace _
{
using namespace std;
using namespace chrono;
using namespace executor;
using namespace utils;
using namespace sync;

struct Inner
{
	void wakeup() {}
	size_t add_source( uv::Evented const &source )
	{
		size_t key = 1;
		poll.reg( source, key );
		return key;
	}

	mpsc::Queue<Box<Future<>>> tasks;
	// Slab<> io_dispatch;
	uv::Poll poll;
};

struct Handle final : Unpark
{
	Handle( Arc<Inner> const &_ ) :
	  _( _ ) {}

	void unpark() const override { _->wakeup(); }

private:
	Arc<Inner> _;
};

struct Reactor final : Park<Handle>
{
	Reactor() = default;

	Handle unpark() const override { return Handle( _ ); }
	void park() override { this->poll( nullptr ); }
	void park( nanoseconds const &timeout ) override { this->poll( &timeout ); }

	bool idle() const { return _->poll.idle(); }
	void with( function<void()> const &fn ) { uv::Poll::current().with( this->_->poll, fn ); }
	void poll( nanoseconds const *timeout = nullptr ) { this->_->poll.poll( events ); }

private:
	Arc<Inner> _ = Arc<Inner>( new Inner );
	uv::Events events;
};

}  // namespace _

using _::Reactor;

}  // namespace reactor

}  // namespace koi
