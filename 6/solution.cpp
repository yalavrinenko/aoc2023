//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <deque>

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
         vw::transform([](const auto &e) { return AOC_Input{e}; }) |
         as_vector;
}

auto is_unique(const auto &q) {
  std::array<char, 'z' - 'a' + 1> map{};
  map.fill(0);

  for (auto e: q) ++map[e - 'a'];
  for (auto e: q)
    if (map[e - 'a'] > 1) return false;

  return true;
}

auto message_start(const auto &message, unsigned unique_length) {
  std::deque<char> window{message.begin(),
                          message.begin() + unique_length - 1};
  for (auto i = unique_length;
       auto c: message | vw::drop(unique_length - 1)) {
    window.emplace_back(c);
    if (is_unique(window)) return AOC_Output{i};
    ++i;
    window.pop_front();
  }
  throw std::logic_error{"No unique region"};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return AOC_Output{message_start(v.front().value, 4)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{message_start(v.front().value, 14)};
}

#include "../exec.hpp"