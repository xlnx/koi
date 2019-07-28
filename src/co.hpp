#pragma once

#include <vector>
#include <deque>
#include <thread>
#include <memory>
#include <future>
#include <type_traits>

#include <result.hpp>

namespace co
{

namespace _
{

template <typename T, typename E>
struct InnerFuture
{
    using Future = std::future<co::Result<T, E>>;
    InnerFuture(Future &&future) : _(std::move(future))
    {
    }

    Future _;
};

} // namespace _

template <typename T, typename E>
struct Future
{
    struct FromArgs
    {
    };

    template <typename... Args>
    Future(FromArgs, Args &&... args) : _(std::make_shared<_::InnerFuture<T, E>>(
                                            std::forward<Args>(args)...))
    {
    }

    template <typename F>
    decltype(auto) and_then(F task)
    {
        using TaskFn = typename tr::InferFunction<F>::type;
        return AndThen::and_then(TaskFn(task));
    }

private:
    struct AndThen
    {
        template <typename U>
        static Future<U, E>
        and_then(std::function<co::Result<U, E>(T)> task)
        {
            return Future<U, E>();
        }
    };

private:
    std::shared_ptr<_::InnerFuture<T, E>> _;
};

namespace _
{

struct WorkItem
{
    WorkItem(std::function<void()> const &task) : task(task)
    {
    }

    std::function<void()> task;
};

struct Co
{
    void run()
    {
    }

    template <typename T, typename E>
    Future<T, E> spawn(std::function<co::Result<T, E>()> task)
    {
        const auto future = Future<T, E>(
            typename Future<T, E>::FromArgs{},
            std::async(std::launch::deferred, task));
        tasks.emplace_back([future]() mutable {

        });
        return future;
    }

private:
    std::deque<WorkItem> tasks;
};

inline std::unique_ptr<Co> &_()
{
    static std::unique_ptr<Co> _;
    return _;
}

} // namespace _

namespace _
{
}

inline void run()
{

    if (_::_() == nullptr)
    {
        _::_().reset(new _::Co);
    }
    else
    {
        //
    }

    _::_()->run();
}

template <typename F>
inline decltype(auto) spawn(F task)
{
    using TaskFn = typename tr::InferFunction<F>::type;
    return _::_()->spawn(TaskFn(task));
}

} // namespace co