#pragma once

#include <memory>
#include <utility>
#include <atomic>
#include <cstddef>
#include <type_traits>

#include <traits/concepts.hpp>
#include <utils/nonnull.hpp>

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
using namespace utils;

template <typename T>
struct Node final : NoCopy, NoMove
{
	T _;
	atomic<Node *> next;

	template <typename... Args>
	Node( Args &&... args ) :
	  _( std::forward<Args>( args )... )
	{
		next.store( nullptr );
	}
};

template <typename T>
struct StubNode final : NoCopy, NoMove
{
	typename aligned_storage<sizeof( T ), alignof( T )>::type _;
	atomic<Node<T> *> next;

	StubNode() { next.store( nullptr ); }
};

template <typename T>
struct Queue final : NoCopy
{
	static_assert( sizeof( StubNode<T> ) == sizeof( Node<T> ),
				   "stub node size != node size" );
	static_assert( offsetof( Node<T>, next ) == offsetof( StubNode<T>, next ),
				   "stub node layout != node layout" );

	Queue()
	{
		head.store( reinterpret_cast<Node<T> *>( &stub ) );
	}
	Queue( Queue &&other )
	{
		if ( !other.empty() ) {
			swap_rvalue_non_empty( std::forward<Queue>( other ) );
		} else {
			head.store( reinterpret_cast<Node<T> *>( &stub ) );
		}
	}
	Queue &operator=( Queue &&other )
	{
		this->~Queue();
		if ( !other.empty() ) {
			swap_rvalue_non_empty( std::forward<Queue>( other ) );
		} else {
			head.store( reinterpret_cast<Node<T> *>( &stub ) );
			stub.next.store( nullptr );
		}
		return *this;
	}
	~Queue()
	{
		auto cur = stub.next.load();
		while ( cur != nullptr ) {
			auto next = cur->next.load();
			auto _ = Box<Node<T>>( cur );
			cur = next;
		}
	}

	template <typename... Args>
	void emplace( Args &&... args )
	{
		auto node = new Node<T>( std::forward<Args>( args )... );
		auto prev = head.exchange( node );
		prev->next.store( node );
	}
	T pop()
	{
		auto next = stub.next.load();
		if ( next != nullptr ) {
			auto next_ptr = next->next.load();
			stub.next.store( next_ptr );
			T ret( std::move( next->_ ) );
			auto _ = Box<Node<T>>( next );
			if ( next_ptr == nullptr ) {
				head.store( reinterpret_cast<Node<T> *>( &stub ) );
			}
			return ret;
		}
		assert( false && "failed to pop an empty queue" );
	}
	bool empty() const
	{
		return head.load() == reinterpret_cast<Node<T> const *>( &stub );
	}

private:
	void swap_rvalue_non_empty( Queue &&other )
	{
		head.store( other.head.load() );
		other.head.store( reinterpret_cast<Node<T> *>( &other.stub ) );
		stub.next.store( other.stub.next.load() );
		other.stub.next.store( nullptr );
	}

private:
	atomic<Node<T> *> head;
	StubNode<T> stub;
};

}  // namespace _

using _::Queue;

}  // namespace mpsc

}  // namespace sync

}  // namespace koi
