#pragma once

#include <atomic>
#include <future/utils/poll_fn.hpp>

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
template <typename Self>
struct Gather;
struct Sink;

struct SinkHandle
{
private:
	template <typename Self>
	friend struct Gather;
	friend struct Sink;

	atomic<bool> err = false;
	atomic<size_t> left = 0;
};

struct Sink : Future<bool>
{
	using Handle = SinkHandle;
	PollState poll() override
	{
		if ( !_->left.load() ) {
			return PollState::Ok;
		} else {
			return PollState::Pending;
		}
	}
	bool get() override { return !_->err; }
	Arc<Handle> handle() const { return _; }

private:
	template <typename Self>
	friend struct Gather;

	Arc<Handle> _ = Arc<Handle>( new Handle );
};

template <typename Self>
struct Gather
{
	static auto gather( Self &&self, Arc<Sink::Handle> const &sink )
	{
		++sink->left;
		using O = typename Self::Output;
		return poll_fn<O>(
		  [self = std::move( self ),
		   sink = sink]( Option<O> &_ ) mutable -> PollState {
			  switch ( auto poll = self.poll() ) {
			  case PollState::Ok:
				  _ = std::move( self.get() );
				  --sink->left;
				  return poll;
			  case PollState::Pruned:
				  --sink->left;
				  sink->err.store( true );
			  default: return poll;
			  }
		  } );
	}
};

template <typename Self>
auto gather( Self &&self, Arc<Sink::Handle> const &sink )
{
	return Gather<Self>::gather( std::forward<Self>( self ), sink );
}

inline auto sink()
{
	return FutureExt<Sink>( Sink() );
}

}  // namespace _

using _::sink;

}  // namespace utils

}  // namespace future

}  // namespace koi
