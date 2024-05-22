// JSON test for list objects only
// Created by Yu Houcheng on 2024/5/22.

#include <gtest/gtest.h>
#include "json.hpp"

TEST(JSONParsingTest, ParseBasicTypes) {
std::string jsonIntString = R"([123, 234, 999, 0, -3])";

hollins::JSON jsonParser = hollins::JSON::parse(jsonIntString);

auto nums = jsonParser.toArray();
ASSERT_EQ(nums.size(), 5);
EXPECT_EQ(nums[0].toInt(), 123);
EXPECT_EQ(nums[1].toInt(), 234);
EXPECT_EQ(nums[2].toInt(), 999);
EXPECT_EQ(nums[3].toInt(), 0);
EXPECT_EQ(nums[5].toInt(), -3);
}
