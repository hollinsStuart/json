#include <gtest/gtest.h>
#include "json.hpp"

TEST(JSONTest, Parse) {
    std::string jsonString = R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false,
        "grades": [85, 90, 78],
        "address": {
            "street": "123 Main St",
            "city": "Anytown"
        }
    })";

    hollins::JSON jsonParser = hollins::JSON::parse(jsonString);

    EXPECT_EQ(jsonParser["name"].toString(), "John Doe");
    EXPECT_EQ(jsonParser["age"].toInt(), 30);
    EXPECT_EQ(jsonParser["isStudent"].toBool(), false);

    auto grades = jsonParser["grades"].toArray();
    ASSERT_EQ(grades.size(), 3);
    EXPECT_EQ(grades[0].toInt(), 85);
    EXPECT_EQ(grades[1].toInt(), 90);
    EXPECT_EQ(grades[2].toInt(), 78);

    auto address = jsonParser["address"].toObject();
    EXPECT_EQ(address.at("street").toString(), "123 Main St");
    EXPECT_EQ(address.at("city").toString(), "Anytown");
}

TEST(JSONTest, Serialization) {
    hollins::JSON jsonObject;
    jsonObject.add("name", "John Doe");
    jsonObject.add("age", 30);
    jsonObject.add("isStudent", false);

    hollins::JSON::Array grades = {85, 90, 78};
    jsonObject.add("grades", grades);

    hollins::JSON::Object address;
    address["street"] = "123 Main St";
    address["city"] = "Anytown";
    jsonObject.add("address", address);

    std::string jsonString = jsonObject.serialize();
    hollins::JSON parsedObject = hollins::JSON::parse(jsonString);

    EXPECT_EQ(parsedObject["name"].toString(), "John Doe");
    EXPECT_EQ(parsedObject["age"].toInt(), 30);
    EXPECT_EQ(parsedObject["isStudent"].toBool(), false);

    auto parsedGrades = parsedObject["grades"].toArray();
    ASSERT_EQ(parsedGrades.size(), 3);
    EXPECT_EQ(parsedGrades[0].toInt(), 85);
    EXPECT_EQ(parsedGrades[1].toInt(), 90);
    EXPECT_EQ(parsedGrades[2].toInt(), 78);

    auto parsedAddress = parsedObject["address"].toObject();
    EXPECT_EQ(parsedAddress.at("street").toString(), "123 Main St");
    EXPECT_EQ(parsedAddress.at("city").toString(), "Anytown");
}

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
