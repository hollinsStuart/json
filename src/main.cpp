// main.cpp
// Created by Yu Houcheng on 5/19/24.

#include <iostream>
#include "json.hpp"

void testPrettyPrint();

int main() {
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

  // Serialize the JSON object to a string
  std::string jsonString = json.serialize();
  std::cout << "Serialized JSON: " << jsonString << std::endl;

  // Parse a JSON string
  std::string jsonString2 = R"({
        "name": "Jane Doe",
        "age": 25,
        "isStudent": true,
        "grades": [95, 88, 92],
        "address": {
            "street": "456 Another St",
            "city": "Othertown"
        }
    })";
  hollins::JSON parsedJson = hollins::JSON::parse(jsonString2);

  // Access and print data from the parsed JSON
  std::cout << "Name: " << parsedJson["name"].toString() << std::endl;
  std::cout << "Age: " << parsedJson["age"].toInt() << std::endl;
  std::cout << "Is Student: " << (parsedJson["isStudent"].toBool() ? "true" : "false") << std::endl;

  std::cout << "Grades: ";
  for (const auto& grade : parsedJson["grades"].toArray()) {
    std::cout << grade.toInt() << " ";
  }
  std::cout << std::endl;

  std::cout << "Address: " << parsedJson["address"]["street"].toString()
            << ", " << parsedJson["address"]["city"].toString() << std::endl;

  // Modify the JSON object
  parsedJson["name"] = "John Smith";
  parsedJson["age"] = 28;
  parsedJson["grades"].add(99);

  // Serialize the modified JSON object to a string
  std::string modifiedJsonString = parsedJson.serialize();
  std::cout << "Modified JSON: " << modifiedJsonString << std::endl;

  testPrettyPrint();
  return 0;
}


void testPrettyPrint() {
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

  // Serialize the JSON object to a string
  std::string jsonString = json.serialize();
  std::cout << "Serialized JSON: " << jsonString << std::endl;

  // Pretty-print the JSON object
  std::cout << "Pretty-printed JSON:\n";
  json.prettyPrint(std::cout);
  std::cout << std::endl;
}
