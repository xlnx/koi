#pragma once

#include <type_traits>
#include <functional>
#include <tuple>

namespace koi
{
namespace traits
{
template <typename T>
struct Helper;

template <typename T>
struct HelperImpl : Helper<decltype( &T::operator() )>
{
};

template <typename T>
struct Helper : HelperImpl<typename std::decay<T>::type>
{
};

template <typename Ret, typename Cls, typename... Args>
struct Helper<Ret ( Cls::* )( Args... )>
{
	using return_type = Ret;
	using argument_type = std::tuple<Args...>;
};

template <typename Ret, typename Cls, typename... Args>
struct Helper<Ret ( Cls::* )( Args... ) const>
{
	using return_type = Ret;
	using argument_type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct Helper<R( Args... )>
{
	using return_type = R;
	using argument_type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct Helper<R ( * )( Args... )>
{
	using return_type = R;
	using argument_type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct Helper<R ( *const )( Args... )>
{
	using return_type = R;
	using argument_type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct Helper<R ( *volatile )( Args... )>
{
	using return_type = R;
	using argument_type = std::tuple<Args...>;
};

template <typename F>
struct InvokeResultOf
{
	using type = typename Helper<F>::return_type;
};

template <typename F>
struct ArgumentTypeOf
{
	using type = typename Helper<F>::argument_type;
};

template <typename Ret, typename Args>
struct InferFunctionAux
{
};

template <typename Ret, typename... Args>
struct InferFunctionAux<Ret, std::tuple<Args...>>
{
	using type = std::function<Ret( Args... )>;
};

template <typename F>
struct InferFunction
{
	using type = typename InferFunctionAux<
	  typename InvokeResultOf<F>::type,
	  typename ArgumentTypeOf<F>::type>::type;
};

}  // namespace traits

}  // namespace koi
