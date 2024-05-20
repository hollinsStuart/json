// json_prettyprint_test.cpp
// Created by Yu Houcheng on 5/20/24.

#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "json.hpp"

// Function to remove ANSI escape codes for comparison
std::string removeAnsiCodes(const std::string& str) {
  std::string result;
  bool skip = false;
  for (char ch : str) {
    if (ch == '\033') {
      skip = true;
    } else if (skip && ch == 'm') {
      skip = false;
    } else if (!skip) {
      result += ch;
    }
  }
  return result;
}

TEST(JSONPrettyPrintTest, PrettyPrint) {
  // Create a JSON object
  hollins::JSON::Object jsonObject;  // Initialize as an Object
  hollins::JSON json(jsonObject);    // Explicitly initialize as an Object

  json.add("name", "John Doe");
  json.add("age", 30);
  json.add("isStudent", false);

  hollins::JSON::Array grades = {85, 90, 78};
  json.add("grades", grades);

  hollins::JSON::Object address;
  address["street"] = "123 Main St";
  address["city"] = "Anytown";
  json.add("address", address);

  // Pretty-print the JSON object
  std::ostringstream oss;
  json.prettyPrint(oss);
  std::string prettyPrintedJSON = oss.str();

  // Remove ANSI codes for parsing
  std::string cleanJSON = removeAnsiCodes(prettyPrintedJSON);

  // Parse back to JSON object to compare content
  hollins::JSON parsedBack = hollins::JSON::parse(cleanJSON);

  // Expected JSON object
  hollins::JSON::Object expectedObject;
  hollins::JSON expectedJSON(expectedObject);

  expectedJSON.add("name", "John Doe");
  expectedJSON.add("age", 30);
  expectedJSON.add("isStudent", false);
  expectedJSON.add("grades", grades);
  expectedJSON.add("address", address);

  // Compare the parsed back JSON with the expected JSON
  EXPECT_EQ(parsedBack["name"].toString(), expectedJSON["name"].toString());
  EXPECT_EQ(parsedBack["age"].toInt(), expectedJSON["age"].toInt());
  EXPECT_EQ(parsedBack["isStudent"].toBool(), expectedJSON["isStudent"].toBool());
  EXPECT_EQ(parsedBack["grades"].toArray().size(), expectedJSON["grades"].toArray().size());
  for (size_t i = 0; i < grades.size(); ++i) {
    EXPECT_EQ(parsedBack["grades"].toArray()[i].toInt(), expectedJSON["grades"].toArray()[i].toInt());
  }
  EXPECT_EQ(parsedBack["address"]["street"].toString(), expectedJSON["address"]["street"].toString());
  EXPECT_EQ(parsedBack["address"]["city"].toString(), expectedJSON["address"]["city"].toString());
}
