#include <string>
#include <gtest/gtest.h>

#include <result.hpp>

TEST(test_result, result_ok)
{
    auto res = co::Result<int, std::string>::Ok(1);
}