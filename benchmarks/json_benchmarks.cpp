// json_benchmarks.cpp
// Created by Yu Houcheng on 5/19/24.

#include <benchmark/benchmark.h>
#include "json.hpp"
#include <nlohmann/json.hpp>

// Benchmark for parsing JSON with hollins::JSON
static void BM_HollinsJSON_Parse(benchmark::State& state) {
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
  for (auto _ : state) {
    hollins::JSON json = hollins::JSON::parse(jsonString);
    benchmark::DoNotOptimize(json);
  }
}
BENCHMARK(BM_HollinsJSON_Parse);

// Benchmark for serialization with hollins::JSON
static void BM_HollinsJSON_Serialize(benchmark::State& state) {
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

  for (auto _ : state) {
    std::string jsonString = jsonObject.serialize();
    benchmark::DoNotOptimize(jsonString);
  }
}
BENCHMARK(BM_HollinsJSON_Serialize);

// Benchmark for parsing JSON with nlohmann/json
static void BM_NlohmannJSON_Parse(benchmark::State& state) {
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
  for (auto _ : state) {
    nlohmann::json json = nlohmann::json::parse(jsonString);
    benchmark::DoNotOptimize(json);
  }
}
BENCHMARK(BM_NlohmannJSON_Parse);

// Benchmark for serialization with nlohmann/json
static void BM_NlohmannJSON_Serialize(benchmark::State& state) {
  nlohmann::json jsonObject = {
      {"name", "John Doe"},
      {"age", 30},
      {"isStudent", false},
      {"grades", {85, 90, 78}},
      {"address", {{"street", "123 Main St"}, {"city", "Anytown"}}}
  };

  for (auto _ : state) {
    std::string jsonString = jsonObject.dump();
    benchmark::DoNotOptimize(jsonString);
  }
}
BENCHMARK(BM_NlohmannJSON_Serialize);

BENCHMARK_MAIN();
