// json_tests.cpp
// Created by Yu Houcheng on 5/19/24.

#include <gtest/gtest.h>
#include "json.hpp"

TEST(JSONTest, AccessAndModification) {
    hollins::JSON jsonObject = hollins::JSON::parse(R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false
    })");

    jsonObject["name"] = "Jane Doe";
    jsonObject["age"] = 25;
    jsonObject["isStudent"] = true;

    EXPECT_EQ(jsonObject["name"].toString(), "Jane Doe");
    EXPECT_EQ(jsonObject["age"].toInt(), 25);
    EXPECT_EQ(jsonObject["isStudent"].toBool(), true);
}

TEST(JSONTest, AddAndRemove) {
    hollins::JSON jsonObject;
    jsonObject.add("name", "John Doe");
    jsonObject.add("age", 30);

    EXPECT_EQ(jsonObject["name"].toString(), "John Doe");
    EXPECT_EQ(jsonObject["age"].toInt(), 30);

    jsonObject.remove("age");
    EXPECT_FALSE(jsonObject["age"].isInt());
}
