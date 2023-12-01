//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <regex>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string value;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines |
         vw::transform([](const auto &s) { return AOC_Input{s}; }) |
         as_vector;
}

auto fix_string(const std::string &s, const std::string &pattern) {
  std::regex regx(pattern);
  auto begin = std::sregex_iterator(s.begin(), s.end(), regx);
  auto end = std::sregex_iterator();
  std::string transformed;
  for (auto it = begin; it != end; ++it) { transformed += it->str(); }
  return transformed;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  const auto *pattern = "1|2|3|4|5|6|7|8|9|0";

  auto input = v | vw::transform([pattern](auto e) {
                 return fix_string(e.value, pattern);
               }) |
               vw::transform([](const auto &s) {
                 return (s.front() - '0') * 10U + (s.back() - '0');
               }) |
               vw::common;
  return AOC_Output{std::accumulate(input.begin(), input.end(), 0U)};
}

auto transform_line(std::string &line) {
  static constexpr std::array<
      std::pair<std::string_view, std::string_view>, 9>
      dmap{
          std::pair<std::string_view, std::string_view>{"one", "o1e"},
          {"two", "t2o"},
          {"three", "t3ree"},
          {"four", "f4ur"},
          {"five", "f5ve"},
          {"six", "s6ze"},
          {"seven", "s7ven"},
          {"eight", "e8ght"},
          {"nine", "n9ne"},
      };
  for (auto &[key, value]: dmap) {
    while (line.find(key) != std::string::npos) {
      line = line.replace(line.find(key), key.size(), value);
    }
  }
  return line;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto input = v | vw::transform([](auto e) {
                 e.value = transform_line(e.value);
                 return e;
               }) |
               as_vector;
  return part_1(input);
}

#include "../exec.hpp"