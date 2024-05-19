// json_serialization_tests.cpp
// Created by Yu Houcheng on 5/19/24.

#include <gtest/gtest.h>
#include "json.hpp"

TEST(JSONSerializationTest, SerializeBasicTypes) {
hollins::JSON jsonObject;
jsonObject.add("name", "John Doe");
jsonObject.add("age", 30);
jsonObject.add("isStudent", false);

std::string jsonString = jsonObject.serialize();
hollins::JSON parsedObject = hollins::JSON::parse(jsonString);

EXPECT_EQ(parsedObject["name"].toString(), "John Doe");
EXPECT_EQ(parsedObject["age"].toInt(), 30);
EXPECT_EQ(parsedObject["isStudent"].toBool(), false);
}

TEST(JSONSerializationTest, SerializeArray) {
hollins::JSON jsonObject;
hollins::JSON::Array grades = {85, 90, 78};
jsonObject.add("grades", grades);

std::string jsonString = jsonObject.serialize();
hollins::JSON parsedObject = hollins::JSON::parse(jsonString);

auto parsedGrades = parsedObject["grades"].toArray();
ASSERT_EQ(parsedGrades.size(), 3);
EXPECT_EQ(parsedGrades[0].toInt(), 85);
EXPECT_EQ(parsedGrades[1].toInt(), 90);
EXPECT_EQ(parsedGrades[2].toInt(), 78);
}
