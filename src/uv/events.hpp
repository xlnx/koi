#pragma once

#include <vector>
#include <functional>
#include <uv.h>

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

struct Event final
{
};

struct Poll;

struct Events final : NoCopy
{
	using Item = Events;
	using Inner = vector<Item>;

	Events( size_t capacity = 1024 ) { _.reserve( capacity ); }

	typename Inner::iterator begin() { return _.begin(); }
	typename Inner::iterator end() { return _.end(); }

	size_t capacity() const { return _.capacity(); }
	size_t size() const { return _.size(); }
	size_t full() const { return size() == capacity(); }
	size_t empty() const { return size(); }

	Events &for_each( function<void( Event & )> const &fn )
	{
		// for ( auto &e : _ ) fn( e );
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
