#ifndef JSON_HPP
#define JSON_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <variant>
#include <string>
#include <string_view>
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
  static JSON parse(std::string_view jsonString) {
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
    if (!isObject()) throw std::runtime_error("Not an object: attempted to access key '" + key + "'");
    return std::get<Object>(value)[key];
  }

  const JSON& operator[](const std::string& key) const {
    if (!isObject()) throw std::runtime_error("Not an object: attempted to access key '" + key + "'");
    return std::get<Object>(value).at(key);
  }

  JSON& operator[](size_t index) {
    if (!isArray()) throw std::runtime_error("Not an array: attempted to access index " + std::to_string(index));
    return std::get<Array>(value)[index];
  }

  const JSON& operator[](size_t index) const {
    if (!isArray()) throw std::runtime_error("Not an array: attempted to access index " + std::to_string(index));
    return std::get<Array>(value).at(index);
  }

  // Overload the assignment operator for JSON objects
  JSON& operator=(const JSON& other) {
    value = other.value;
    return *this;
  }

  // Overload the assignment operator for JSON objects to accept different value types
  JSON& operator=(const std::string& s) {
    value = s;
    return *this;
  }

  JSON& operator=(const char* s) {
    value = std::string(s);
    return *this;
  }

  JSON& operator=(bool b) {
    value = b;
    return *this;
  }

  JSON& operator=(int i) {
    value = i;
    return *this;
  }

  JSON& operator=(double d) {
    value = d;
    return *this;
  }

  // Add methods
  void add(const std::string& key, const JSON& value) {
    if (!isObject()) throw std::runtime_error("Not an object: attempted to add key '" + key + "'");
    std::get<Object>(this->value)[key] = value;
  }

  void add(const JSON& value) {
    if (!isArray()) throw std::runtime_error("Not an array: attempted to add value");
    std::get<Array>(this->value).push_back(value);
  }

  void remove(const std::string& key) {
    if (!isObject()) throw std::runtime_error("Not an object: attempted to remove key '" + key + "'");
    std::get<Object>(value).erase(key);
  }

  void remove(size_t index) {
    if (!isArray()) throw std::runtime_error("Not an array: attempted to remove index " + std::to_string(index));
    auto& array = std::get<Array>(value);
    if (index >= array.size()) throw std::runtime_error("Index out of bounds: attempted to remove index " + std::to_string(index));
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

  // Pretty-printing with colors
  void prettyPrint(std::ostream& os, int indent = 0) const {
    std::string indentStr(indent, ' ');
    std::visit([&os, &indentStr, indent](auto&& arg) {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, std::nullptr_t>) {
        os << "\033[31mnull\033[0m";  // Red
      } else if constexpr (std::is_same_v<T, bool>) {
        os << "\033[35m" << (arg ? "true" : "false") << "\033[0m";  // Magenta
      } else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
        os << "\033[33m" << arg << "\033[0m";  // Yellow
      } else if constexpr (std::is_same_v<T, std::string>) {
        os << "\033[32m\"" << arg << "\"\033[0m";  // Green
      } else if constexpr (std::is_same_v<T, Object>) {
        os << "{\n";
        bool first = true;
        for (const auto& [key, value] : arg) {
          if (!first) os << ",\n";
          os << indentStr << "  \033[36m\"" << key << "\"\033[0m: ";
          value.prettyPrint(os, indent + 2);
          first = false;
        }
        os << "\n" << indentStr << "}";
      } else if constexpr (std::is_same_v<T, Array>) {
        os << "[\n";
        bool first = true;
        for (const auto& value : arg) {
          if (!first) os << ",\n";
          os << indentStr << "  ";
          value.prettyPrint(os, indent + 2);
          first = false;
        }
        os << "\n" << indentStr << "]";
      }
    }, value);
  }

 private:
  Value value;

  static void skipWhitespace(std::string_view jsonString, size_t& pos) {
    while (pos < jsonString.size() && std::isspace(jsonString[pos])) ++pos;
  }

  static JSON parseValue(std::string_view jsonString, size_t& pos) {
    skipWhitespace(jsonString, pos);
    switch (jsonString[pos]) {
      case '{': return parseObject(jsonString, pos);
      case '[': return parseArray(jsonString, pos);
      case '"': return parseString(jsonString, pos);
      case 't': case 'f': return parseBoolean(jsonString, pos);
      case 'n': return parseNull(jsonString, pos);
      default: return parseNumber(jsonString, pos);
    }
  }

  static JSON parseObject(std::string_view jsonString, size_t& pos) {
    Object object;
    ++pos; // skip '{'
    skipWhitespace(jsonString, pos);
    while (pos < jsonString.size() && jsonString[pos] != '}') {
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
    if (pos == jsonString.size() || jsonString[pos] != '}') throw std::runtime_error("Expected '}' in object");
    ++pos; // skip '}'
    return JSON(object);
  }

  static JSON parseArray(std::string_view jsonString, size_t& pos) {
    Array array;
    ++pos; // skip '['
    skipWhitespace(jsonString, pos);
    while (pos < jsonString.size() && jsonString[pos] != ']') {
      JSON value = parseValue(jsonString, pos);
      array.push_back(value);
      skipWhitespace(jsonString, pos);
      if (jsonString[pos] == ',') ++pos; // skip ','
      skipWhitespace(jsonString, pos);
    }
    if (pos == jsonString.size() || jsonString[pos] != ']') throw std::runtime_error("Expected ']' in array");
    ++pos; // skip ']'
    return JSON(array);
  }

  static JSON parseString(std::string_view jsonString, size_t& pos) {
    ++pos; // skip '"'
    std::string result;
    while (pos < jsonString.size() && jsonString[pos] != '"') {
      if (jsonString[pos] == '\\') {
        ++pos; // skip '\'
        switch (jsonString[pos]) {
          case '"': result += '"'; break;
          case '\\': result += '\\'; break;
          case '/': result += '/'; break;
          case 'b': result += '\b'; break;
          case 'f': result += '\f'; break;
          case 'n': result += '\n'; break;
          case 'r': result += '\r'; break;
          case 't': result += '\t'; break;
          default: throw std::runtime_error("Invalid escape sequence");
        }
      } else {
        result += jsonString[pos];
      }
      ++pos;
    }
    if (pos == jsonString.size() || jsonString[pos] != '"') throw std::runtime_error("Expected '\"' in string");
    ++pos; // skip closing '"'
    return JSON(result);
  }

  static JSON parseNumber(std::string_view jsonString, size_t& pos) {
    size_t start = pos;
    if (jsonString[pos] == '-') ++pos;
    while (pos < jsonString.size() && std::isdigit(jsonString[pos])) ++pos;
    if (pos < jsonString.size() && jsonString[pos] == '.') {
      ++pos;
      while (pos < jsonString.size() && std::isdigit(jsonString[pos])) ++pos;
      return JSON(std::stod(std::string(jsonString.substr(start, pos - start))));
    }
    return JSON(std::stoi(std::string(jsonString.substr(start, pos - start))));
  }

  static JSON parseBoolean(std::string_view jsonString, size_t& pos) {
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

  static JSON parseNull(std::string_view jsonString, size_t& pos) {
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
