#pragma once

#include <tuple>
#include <iostream>
#include <array>
#include <type_traits>

#include "poll_fn.hpp"

namespace koi
{
namespace future
{
namespace utils
{
namespace _
{
using namespace std;

template <size_t I0, size_t... I>
struct PrintSeq
{
	static void print( ostream &os )
	{
		os << I0 << ", ";
		PrintSeq<I...>::print( os );
	}
};

template <size_t I>
struct PrintSeq<I>
{
	static void print( ostream &os )
	{
		os << I;
	}
};

template <size_t... I>
struct IntSeq
{
};

template <size_t... I>
ostream &operator<<( ostream &os, IntSeq<I...> i )
{
	PrintSeq<I...>::print( os );
	return os;
}

template <size_t N, size_t... S>
struct MakeSeq : MakeSeq<N - 1, N - 1, S...>
{
};

template <size_t... S>
struct MakeSeq<0, S...>
{
	using type = IntSeq<S...>;
};

struct Zipped
{
	template <typename... A, typename... B>
	static auto zip( tuple<A...> &a, tuple<B...> &b )
	{
		return zip_impl( a, b, typename MakeSeq<sizeof...( A )>::type{} );
	}

	template <typename... A, typename... B, size_t... I>
	static auto zip_impl( tuple<A...> &a, tuple<B...> &b, IntSeq<I...> )
	{
		return make_tuple( make_pair<
						   typename std::decay<A>::type *,
						   typename std::decay<B>::type *>(
		  &get<I>( a ), &get<I>( b ) )... );
	}
};

struct CollectSeq
{
	template <typename... Futs, typename... Ret>
	static void collect( tuple<Futs...> &futs, Option<tuple<Ret...>> &rets )
	{
		rets = collect_impl( futs, typename MakeSeq<sizeof...( Futs )>::type{} );
	}

	template <typename... Futs, size_t... I>
	static auto collect_impl( tuple<Futs...> &futs, IntSeq<I...> )
	{
		return make_tuple( ( typename Futs::Output )( std::move( get<I>( futs ).get() ) )... );
	}
};

struct PollSeq
{
	template <typename... Futs, typename... Done>
	static bool poll( tuple<Futs...> &futs, tuple<Done...> &done )
	{
		auto zipped = Zipped::zip( futs, done );
		return poll_impl( zipped, typename MakeSeq<sizeof...( Futs )>::type{} );
	}

	template <typename... A, size_t... I>
	static bool poll_impl( tuple<A...> &polls, IntSeq<I...> i )
	{
		array<bool, sizeof...( A )> _ = { poll_one( get<I>( polls ) )... };
		for ( auto done : _ ) {
			if ( !done ) return false;
		}
		return true;
	}

	template <typename Fut, typename Done>
	static bool poll_one( pair<Fut, Done> &poll )
	{
		if ( *poll.second ) return true;
		if ( poll.first->poll() ) {
			*poll.second = true;
			return true;
		}
		return false;
	}
};

template <typename A, typename T>
bool expand_with( T &&t )
{
	return std::forward<T>( t );
}

template <typename... Futs>
auto join( Futs &&... fs )
{
	using Output = tuple<typename Futs::Output...>;
	auto futs = make_tuple( std::forward<Futs>( fs )... );
	auto done = make_tuple( expand_with<Futs>( false )... );
	return poll_fn<Output>(
	  [futs = std::move( futs ),
	   done = std::move( done )]( Option<Output> &_ ) mutable -> bool {
		  if ( PollSeq::poll( futs, done ) ) {
			  CollectSeq::collect( futs, _ );
			  return true;
		  }
		  return false;
	  } );
}

}  // namespace _

}  // namespace utils

}  // namespace future

}  // namespace koi