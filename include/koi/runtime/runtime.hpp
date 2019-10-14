#pragma once

#include <memory>

// #include "current_thread/runtime.hpp"
#include <future/future.hpp>
#include <traits/concepts.hpp>
#include <utils/with.hpp>

namespace koi
{
namespace runtime
{
namespace _
{
using namespace std;
using namespace traits::concepts;
using namespace koi::utils;

struct Runtime : Dynamic, NoCopy
{
	virtual void spawn( Box<Future<>> &&fut ) = 0;
	virtual void run() = 0;

	void run( Box<Future<>> &&fut )
	{
		this->spawn( std::move( fut ) );
		this->run();
	}
};

template <typename Self>
struct TypedRuntime : Self
{
	template <typename F, typename = typename enable_if<
							is_base_of<Future<>, typename decay<F>::type>::value>::type>
	void spawn( F &&fut )
	{
		using Fut = typename decay<F>::type;
		Self::spawn( Box<Future<>>( new Fut( std::forward<F>( fut ) ) ) );
	}
	template <typename F, typename = typename enable_if<
							is_base_of<Future<>, typename decay<F>::type>::value>::type>
	void run( F &&fut )
	{
		using Fut = typename decay<F>::type;
		Self::spawn( Box<Future<>>( new Fut( std::forward<F>( fut ) ) ) );
		Self::run();
	}
	void spawn( Box<Future<>> &&fut )
	{
		Self::spawn( std::move( fut ) );
	}
	void run()
	{
		Self::run();
	}
	void run( Box<Future<>> &&fut )
	{
		Self::run( std::move( fut ) );
	}
};

struct CurrentRuntime : Runtime, With<Runtime>
{
	void spawn( Box<Future<>> &&fut ) override
	{
		( **this ).spawn( std::move( fut ) );
	}
	void run() override
	{
		( **this ).run();
	}
};

inline TypedRuntime<CurrentRuntime> &current_runtime()
{
	static TypedRuntime<CurrentRuntime> _;
	return _;
}

}  // namespace _

}  // namespace runtime

}  // namespace koi