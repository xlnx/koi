#pragma once

#include <utility>
#include <memory>
#include <atomic>

#include "queue.hpp"
#include "mutex.hpp"
#include <traits/concepts.hpp>

namespace koi
{
namespace sync
{
namespace mpsc
{
namespace _
{
using namespace std;
using namespace traits::concepts;

template <typename T>
struct Channel;

template <typename T>
struct ChannelImpl final : NoCopy, NoMove
{
	Queue<T> message_queue;
	Queue<shared_ptr<Mutex<T>>> parked_queue;
	atomic<size_t> num_senders;
	//  recv_task;
};

template <typename T>
struct SenderImpl
{
	shared_ptr<ChannelImpl<T>> _;
	// shared_ptr<>;
	bool is_parked = false;

	SenderImpl( shared_ptr<ChannelImpl<T>> const &_ ) :
	  _( _ ) {}
};

template <typename T>
struct Sender
{
private:
	Sender( SenderImpl<T> &&_ ) :
	  _( std::forward<SenderImpl<T>>( _ ) ) {}

private:
	SenderImpl<T> _;

	template <typename U>
	friend struct Channel;
};

template <typename T>
struct Receiver
{
private:
	Receiver( shared_ptr<ChannelImpl<T>> const &_ ) :
	  _( _ ) {}

private:
	shared_ptr<ChannelImpl<T>> _;
	template <typename U>
	friend struct Channel;
};

template <typename T>
struct Channel
{
	static pair<Sender<T>, Receiver<T>>
	  channel()
	{
		auto impl = make_shared<ChannelImpl<T>>();
		return { SenderImpl<T>( impl ), Receiver<T>( impl ) };
	}
};

template <typename T>
pair<Sender<T>, Receiver<T>>
  channel()
{
	return Channel<T>::channel();
}

}  // namespace _

using _::channel;

}  // namespace mpsc

}  // namespace sync

}  // namespace koi