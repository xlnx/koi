#pragma once

#include <variant>
#include <fn/function.hpp>

namespace co
{

template <typename T, typename E>
struct Result
{

    template <typename... Args>
    static Result Ok(Args &&... args)
    {
        Result res;
        res._.template emplace<T>(std::forward<Args>(args)...);
        return res;
    }
    template <typename... Args>
    static Result Err(Args &&... args)
    {
        Result res;
        res._.template emplace<E>(std::forward<Args>(args)...);
        return res;
    }

private:
    Result() = default;

private:
    std::variant<T, std::unique_ptr<E>> _;
};

} // namespace co