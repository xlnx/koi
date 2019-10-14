#pragma once

#include <type_traits>

#include <sync/parker.hpp>
#include <traits/concepts.hpp>

namespace koi
{
namespace executor
{
namespace _
{
using namespace std;
using namespace chrono;
using namespace sync;
using namespace traits::concepts;

struct Unpark : Dynamic
{
	virtual void unpark() const = 0;
};

template <typename U>
struct Park : Dynamic
{
	static_assert( is_base_of<_::Unpark, U>::value,
				   "Park<U: Unpark>" );
	using Unpark = U;

	virtual U unpark() const = 0;
	virtual void park() = 0;
	virtual void park( nanoseconds const &timeout ) = 0;
};

struct ParkThread;

struct UnparkThread final : Unpark
{
	void unpark() const override { _.unpark(); }

private:
	UnparkThread( Unparker const &_ ) :
	  _( _ ) {}

private:
	Unparker const &_;
	friend struct ParkThread;
};

struct ParkThread final : Park<UnparkThread>
{
	UnparkThread unpark() const override { return _().unparker(); }
	void park() override { _().park(); }
	void park( nanoseconds const &timeout ) override { _().park( timeout ); }

private:
	static Parker &_()
	{
		thread_local Parker parker;
		return parker;
	}
};

}  // namespace _

using _::Park;
using _::ParkThread;
using _::Unpark;
using _::UnparkThread;

}  // namespace executor

}  // namespace koi