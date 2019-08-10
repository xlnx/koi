#pragma once

#include <vector>
#include <functional>
#include <uv.h>

#include <utils/with.hpp>
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

enum Ready
{
	DEFAULT = 0
};

struct Event final
{
	Event( size_t token, Ready kind = DEFAULT ) :
	  token( token ),
	  kind( kind ) {}

	size_t token;
	Ready kind;
};

struct Poll;

struct Events final : NoCopy
{
	using Item = Event;
	using Inner = vector<Item>;

	Events( size_t capacity = 1024 ) { _.reserve( capacity ); }

	typename Inner::iterator begin() { return _.begin(); }
	typename Inner::iterator end() { return _.end(); }

	size_t capacity() const { return _.capacity(); }
	size_t size() const { return _.size(); }
	size_t full() const { return size() == capacity(); }
	size_t empty() const { return size(); }

	template <typename... Args>
	void emplace( Args &&... args )
	{
		_.emplace_back( std::forward<Args>( args )... );
	}
	Events &for_each( function<void( Event & )> const &fn )
	{
		for ( auto &e : _ ) fn( e );
		return *this;
	}

public:
	static With<Events> &current()
	{
		thread_local With<Events> _;
		return _;
	}

private:
	Inner _;
	friend struct Poll;
};

}  // namespace _

using _::Event;
using _::Events;

}  // namespace uv

}  // namespace koi
