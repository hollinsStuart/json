// json_parsing_tests.cpp
// Created by Yu Houcheng on 5/19/24.

#include <gtest/gtest.h>
#include "json.hpp"

TEST(JSONParsingTest, ParseBasicTypes) {
std::string jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false
    })";

hollins::JSON jsonParser = hollins::JSON::parse(jsonString);

EXPECT_EQ(jsonParser["name"].toString(), "John Doe");
EXPECT_EQ(jsonParser["age"].toInt(), 30);
EXPECT_EQ(jsonParser["isStudent"].toBool(), false);
}

TEST(JSONParsingTest, ParseArray) {
std::string jsonString = R"({
        "grades": [85, 90, 78]
    })";

hollins::JSON jsonParser = hollins::JSON::parse(jsonString);

auto grades = jsonParser["grades"].toArray();
ASSERT_EQ(grades.size(), 3);
EXPECT_EQ(grades[0].toInt(), 85);
EXPECT_EQ(grades[1].toInt(), 90);
EXPECT_EQ(grades[2].toInt(), 78);
}
