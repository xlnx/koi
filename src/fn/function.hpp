#pragma once

#include <type_traits>
#include <functional>
#include <tuple>

namespace co
{

namespace tr
{

template <typename T>
struct InvokeResultOf: InvokeResultOf<decltype(&T::operator())>
{
};

template <typename Ret, typename Cls, typename... Args>
struct InvokeResultOf<Ret(Cls::*)(Args...)>
{
    using type = Ret;
};

template <typename Ret, typename Cls, typename... Args>
struct InvokeResultOf<Ret(Cls::*)(Args...) const>
{
    using type = Ret;
};

template <typename R, typename ...Args>
struct InvokeResultOf<R(Args...)>
{
    using type = R;
};

template <typename R, typename ...Args>
struct InvokeResultOf<R(*)(Args...)>
{
    using type = R;
};

template <typename R, typename ...Args>
struct InvokeResultOf<R(*const)(Args...)>
{
    using type = R;
};

template <typename R, typename ...Args>
struct InvokeResultOf<R(*volatile)(Args...)>
{
    using type = R;
};

template <typename F, typename Ret, typename... Args>
std::tuple<Args...>
helper(Ret (F::*)(Args...));

template <typename F, typename Ret, typename... Args>
std::tuple<Args...>
helper(Ret (F::*)(Args...) const);

template <typename F>
struct ArgumentTypeOf
{
    using type = decltype( helper(&F::operator()) );
};

template <typename Ret, typename Args>
struct InferFunctionAux
{
};

template <typename Ret, typename ...Args>
struct InferFunctionAux<Ret, std::tuple<Args...>>
{
    using type = std::function<Ret(Args...)>;
};

template <typename F>
struct InferFunction
{
    using type = typename InferFunctionAux<
        typename InvokeResultOf<F>::type,
        typename ArgumentTypeOf<F>::type
    >::type;
};

}

}