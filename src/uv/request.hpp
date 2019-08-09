#pragma once

#include <functional>
#include <utility>
#include <uv.h>

#include <utils/nonnull.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace uv
{
namespace _
{
using namespace std;
using namespace utils;
using namespace traits::concepts;

template <typename T>
struct Inner final : NoCopy
{
	Inner( function<void( T * )> &&fn ) :
	  fn( std::move( fn ) )
	{
		// reinterpret_cast<uv_handle_t *>( &this->_ )->data = this;
	}

	T _;
	function<void( T * )> fn;
};

template <typename T>
struct Request : NoCopy
{
	Request( function<void( T * )> &&fn ) :
	  _( std::move( fn ) ) {}

	operator T *() const { return this->get(); }
	T *get() const { return &_._; }

	void reset() const { _.ready = false; }
	bool ready() const { return _.ready; }
	T *raw() const { return _._; }

public:
	static void into_poll( T *_ )
	{
		auto inner = reinterpret_cast<Inner<T> *>( _ );
		// auto inner = static_cast<Inner<T> *>(
		//   reinterpret_cast<uv_handle_t *>( _ )->data );
		inner->fn( _ );
	}

private:
	Inner<T> _;
};

}  // namespace _

}  // namespace uv

}  // namespace koi
