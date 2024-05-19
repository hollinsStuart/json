#ifndef JSON_HPP
#define JSON_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <variant>
#include <string>
#include <stdexcept>
#include <sstream>
#include <type_traits>

namespace hollins {

class JSON {
 public:
  using Object = std::unordered_map<std::string, JSON>;
  using Array = std::vector<JSON>;
  using Value = std::variant<std::nullptr_t, bool, int, double, std::string, Object, Array>;

  // Constructors
  JSON() : value(nullptr) {}
  JSON(bool b) : value(b) {}
  JSON(int i) : value(i) {}
  JSON(double d) : value(d) {}
  JSON(const std::string& s) : value(s) {}
  JSON(const char* s) : value(std::string(s)) {}
  JSON(const Object& o) : value(o) {}
  JSON(const Array& a) : value(a) {}

  // Parsing and Serialization
  static JSON parse(const std::string& jsonString) {
    size_t pos = 0;
    return parseValue(jsonString, pos);
  }

  std::string serialize() const {
    std::ostringstream oss;
    serialize(oss);
    return oss.str();
  }

  // Access and Manipulation
  JSON& operator[](const std::string& key) {
    if (!isObject()) throw std::runtime_error("Not an object");
    return std::get<Object>(value)[key];
  }

  const JSON& operator[](const std::string& key) const {
    if (!isObject()) throw std::runtime_error("Not an object");
    return std::get<Object>(value).at(key);
  }

  JSON& operator[](size_t index) {
    if (!isArray()) throw std::runtime_error("Not an array");
    return std::get<Array>(value)[index];
  }

  const JSON& operator[](size_t index) const {
    if (!isArray()) throw std::runtime_error("Not an array");
    return std::get<Array>(value).at(index);
  }

  void add(const std::string& key, const JSON& value) {
    if (!isObject()) throw std::runtime_error("Not an object");
    std::get<Object>(this->value)[key] = value;
  }

  void add(const JSON& value) {
    if (!isArray()) throw std::runtime_error("Not an array");
    std::get<Array>(this->value).push_back(value);
  }

  void remove(const std::string& key) {
    if (!isObject()) throw std::runtime_error("Not an object");
    std::get<Object>(value).erase(key);
  }

  void remove(size_t index) {
    if (!isArray()) throw std::runtime_error("Not an array");
    auto& array = std::get<Array>(value);
    if (index >= array.size()) throw std::runtime_error("Index out of bounds");
    array.erase(array.begin() + index);
  }

  // Type Checking and Conversion
  bool isNull() const { return std::holds_alternative<std::nullptr_t>(value); }
  bool isBool() const { return std::holds_alternative<bool>(value); }
  bool isInt() const { return std::holds_alternative<int>(value); }
  bool isDouble() const { return std::holds_alternative<double>(value); }
  bool isString() const { return std::holds_alternative<std::string>(value); }
  bool isObject() const { return std::holds_alternative<Object>(value); }
  bool isArray() const { return std::holds_alternative<Array>(value); }

  bool toBool() const {
    if (!isBool()) throw std::runtime_error("Not a bool");
    return std::get<bool>(value);
  }

  int toInt() const {
    if (!isInt()) throw std::runtime_error("Not an int");
    return std::get<int>(value);
  }

  double toDouble() const {
    if (!isDouble()) throw std::runtime_error("Not a double");
    return std::get<double>(value);
  }

  std::string toString() const {
    if (!isString()) throw std::runtime_error("Not a string");
    return std::get<std::string>(value);
  }

  const Object& toObject() const {
    if (!isObject()) throw std::runtime_error("Not an object");
    return std::get<Object>(value);
  }

  const Array& toArray() const {
    if (!isArray()) throw std::runtime_error("Not an array");
    return std::get<Array>(value);
  }

  // Utility Functions
  bool empty() const {
    if (isObject()) return std::get<Object>(value).empty();
    if (isArray()) return std::get<Array>(value).empty();
    throw std::runtime_error("Not an object or array");
  }

  size_t size() const {
    if (isObject()) return std::get<Object>(value).size();
    if (isArray()) return std::get<Array>(value).size();
    throw std::runtime_error("Not an object or array");
  }

  void clear() {
    if (isObject()) std::get<Object>(value).clear();
    if (isArray()) std::get<Array>(value).clear();
    throw std::runtime_error("Not an object or array");
  }

  // Iteration
  Object::iterator beginObject() {
    if (!isObject()) throw std::runtime_error("Not an object");
    return std::get<Object>(value).begin();
  }

  Object::iterator endObject() {
    if (!isObject()) throw std::runtime_error("Not an object");
    return std::get<Object>(value).end();
  }

  Array::iterator beginArray() {
    if (!isArray()) throw std::runtime_error("Not an array");
    return std::get<Array>(value).begin();
  }

  Array::iterator endArray() {
    if (!isArray()) throw std::runtime_error("Not an array");
    return std::get<Array>(value).end();
  }

  template<typename T>
  T get() const {
    return std::get<T>(value);
  }

 private:
  Value value;

  static void skipWhitespace(const std::string& jsonString, size_t& pos) {
    while (std::isspace(jsonString[pos])) ++pos;
  }

  static JSON parseValue(const std::string& jsonString, size_t& pos) {
    skipWhitespace(jsonString, pos);
    if (jsonString[pos] == '{') return parseObject(jsonString, pos);
    if (jsonString[pos] == '[') return parseArray(jsonString, pos);
    if (jsonString[pos] == '"') return parseString(jsonString, pos);
    if (std::isdigit(jsonString[pos]) || jsonString[pos] == '-') return parseNumber(jsonString, pos);
    if (jsonString[pos] == 't' || jsonString[pos] == 'f') return parseBoolean(jsonString, pos);
    if (jsonString[pos] == 'n') return parseNull(jsonString, pos);
    throw std::runtime_error("Invalid JSON value");
  }

  static JSON parseObject(const std::string& jsonString, size_t& pos) {
    Object object;
    ++pos; // skip '{'
    skipWhitespace(jsonString, pos);
    while (jsonString[pos] != '}') {
      skipWhitespace(jsonString, pos);
      std::string key = parseString(jsonString, pos).get<std::string>();
      skipWhitespace(jsonString, pos);
      if (jsonString[pos] != ':') throw std::runtime_error("Expected ':' in object");
      ++pos; // skip ':'
      skipWhitespace(jsonString, pos);
      JSON value = parseValue(jsonString, pos);
      object[key] = value;
      skipWhitespace(jsonString, pos);
      if (jsonString[pos] == ',') ++pos; // skip ','
      skipWhitespace(jsonString, pos);
    }
    ++pos; // skip '}'
    return JSON(object);
  }

  static JSON parseArray(const std::string& jsonString, size_t& pos) {
    Array array;
    ++pos; // skip '['
    skipWhitespace(jsonString, pos);
    while (jsonString[pos] != ']') {
      JSON value = parseValue(jsonString, pos);
      array.push_back(value);
      skipWhitespace(jsonString, pos);
      if (jsonString[pos] == ',') ++pos; // skip ','
      skipWhitespace(jsonString, pos);
    }
    ++pos; // skip ']'
    return JSON(array);
  }

  static JSON parseString(const std::string& jsonString, size_t& pos) {
    ++pos; // skip '"'
    std::string result;
    while (jsonString[pos] != '"') {
      if (jsonString[pos] == '\\') {
        ++pos; // skip '\'
        if (jsonString[pos] == '"') result += '"';
        else if (jsonString[pos] == '\\') result += '\\';
        else if (jsonString[pos] == '/') result += '/';
        else if (jsonString[pos] == 'b') result += '\b';
        else if (jsonString[pos] == 'f') result += '\f';
        else if (jsonString[pos] == 'n') result += '\n';
        else if (jsonString[pos] == 'r') result += '\r';
        else if (jsonString[pos] == 't') result += '\t';
        else throw std::runtime_error("Invalid escape sequence");
      } else {
        result += jsonString[pos];
      }
      ++pos;
    }
    ++pos; // skip closing '"'
    return JSON(result);
  }

  static JSON parseNumber(const std::string& jsonString, size_t& pos) {
    size_t start = pos;
    if (jsonString[pos] == '-') ++pos;
    while (std::isdigit(jsonString[pos])) ++pos;
    if (jsonString[pos] == '.') {
      ++pos;
      while (std::isdigit(jsonString[pos])) ++pos;
      return JSON(std::stod(jsonString.substr(start, pos - start)));
    }
    return JSON(std::stoi(jsonString.substr(start, pos - start)));
  }

  static JSON parseBoolean(const std::string& jsonString, size_t& pos) {
    if (jsonString.substr(pos, 4) == "true") {
      pos += 4;
      return JSON(true);
    }
    if (jsonString.substr(pos, 5) == "false") {
      pos += 5;
      return JSON(false);
    }
    throw std::runtime_error("Invalid boolean value");
  }

  static JSON parseNull(const std::string& jsonString, size_t& pos) {
    if (jsonString.substr(pos, 4) == "null") {
      pos += 4;
      return JSON();
    }
    throw std::runtime_error("Invalid null value");
  }

  void serialize(std::ostream& os) const {
    std::visit([&os](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, std::nullptr_t>) os << "null";
      else if constexpr (std::is_same_v<T, bool>) os << (arg ? "true" : "false");
      else if constexpr (std::is_same_v<T, int>) os << arg;
      else if constexpr (std::is_same_v<T, double>) os << arg;
      else if constexpr (std::is_same_v<T, std::string>) os << "\"" << arg << "\"";
      else if constexpr (std::is_same_v<T, Object>) {
      os << "{";
      bool first = true;
      for (const auto& [key, value] : arg) {
        if (!first) os << ",";
        os << "\"" << key << "\":";
        value.serialize(os);
        first = false;
      }
      os << "}";
    } else if constexpr (std::is_same_v<T, Array>) {
      os << "[";
      bool first = true;
      for (const auto& value : arg) {
        if (!first) os << ",";
        value.serialize(os);
        first = false;
      }
      os << "]";
    }
    }, value);
  }
};

} // namespace hollins

#endif // JSON_HPP
