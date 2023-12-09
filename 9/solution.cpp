//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<int64_t> values;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &s) {
           std::istringstream iss{s};
           AOC_Input in;
           std::copy(std::istream_iterator<int64_t>{iss},
                     std::istream_iterator<int64_t>{},
                     std::back_inserter(in.values));
           return in;
         }) |
         as_vector;
}

std::vector<int64_t> lowering_right(std::vector<int64_t> in) {
  for (auto i = 1; i < in.size(); ++i) {
    for (auto j = 0; j < in.size() - i; ++j) {
      in[j] = in[j + 1] - in[j];
    }
  }
  return in;
}

std::vector<int64_t> lowering_left(std::vector<int64_t> in) {
  std::vector<int64_t> out = in;
  for (auto i = 1; i < in.size(); ++i) {
    for (auto j = i; j < in.size(); ++j) {
      out[j] = in[j] - in[j - 1];
    }
    std::copy(out.begin() + i, out.end(), in.begin() + i);
  }
  return in;
}


AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto lower = v | vw::transform(&AOC_Input::values) |
               vw::transform(lowering_right) |
               vw::transform([](const auto &r) {
                 return std::accumulate(r.begin(), r.end(), 0LL);
               });
  return AOC_Output{std::accumulate(lower.begin(), lower.end(), 0LL)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto lower = v | vw::transform(&AOC_Input::values) |
               vw::transform(lowering_left) |
               vw::transform([](const auto &r) {
                 auto value = 0LL;

                 for (const auto &e: r | vw::reverse) {
                   value = e - value;
                 }

                 return value;
               });
  return AOC_Output{std::accumulate(lower.begin(), lower.end(), 0LL)};
}

#include "../exec.hpp"