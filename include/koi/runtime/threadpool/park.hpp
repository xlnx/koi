#pragma once

#include <utility>
#include <memory>
#include <chrono>
#include <type_traits>

#include <traits/concepts.hpp>
#include <executor/park.hpp>

namespace koi
{
namespace worker
{
namespace _
{
using namespace std;
using namespace chrono;

using namespace executor;
using namespace traits::concepts;

struct UnparkPtr final : Unpark, NoCopy
{
	UnparkPtr() = default;
	template <typename T, typename = typename enable_if<
							is_base_of<Unpark, T>::value>::type>
	UnparkPtr( T &&_ ) :
	  _( new T( std::forward<T>( _ ) ) )
	{
	}

	void unpark() const override
	{
		_->unpark();
	}

	operator bool() const { return _.operator bool(); }
	void clear() const { _.reset( nullptr ); }

private:
	mutable Box<Unpark> _;
};

struct ParkPtr final : Park<UnparkPtr>, NoCopy
{
	using Unpark = UnparkPtr;

	ParkPtr() = default;
	template <typename T, typename = typename enable_if<
							is_base_of<Park, T>::value>::type>
	ParkPtr( T &&_ ) :
	  _( new T( std::std::forward<T>( _ ) ) )
	{
	}

	Unpark unpark() const override { return UnparkPtr( _->unpark() ); }
	void park() override { _->park(); }
	void park( nanoseconds const &timeout ) override { _->park( timeout ); }

	operator bool() const { return _.operator bool(); }
	void clear() const { _.reset( nullptr ); }

private:
	mutable Box<Park> _;
};

}  // namespace _

}  // namespace worker

}  // namespace koi
