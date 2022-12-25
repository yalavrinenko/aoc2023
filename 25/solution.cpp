//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  std::string value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::string> snafu;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  AOC_Input in;
  in.snafu = lines;
  return {in};
}

inline std::unordered_map<char, long> snf_c{{'=', -2},
                                            {'-', -1},
                                            {'0', 0},
                                            {'1', 1},
                                            {'2', 2}};

inline std::unordered_map<long, char> dec_c{{-2, '='},
                                            {-1, '-'},
                                            {0, '0'},
                                            {1, '1'},
                                            {2, '2'}};

auto snafu_to_long(std::string const &snafu) {
  long value = 0;
  long pow5 = 1;
  for (auto c: snafu | vw::reverse) {
    value += snf_c[c] * pow5;
    pow5 *= 5;
  }
  return value;
}

inline std::unordered_map<long, std::pair<long, long>> correction{
    {5, {0, 1}},   {4, {-1, 1}},  {3, {-2, 1}},  {2, {2, 0}},
    {1, {1, 0}},   {0, {0, 0}},   {-1, {-1, 0}}, {-2, {-2, 0}},
    {-3, {2, -1}}, {-4, {1, -1}},
};

auto long_to_snafu(long value) {
  std::vector<long> snafu;
  while (value != 0) {
    snafu.emplace_back(value % 5);
    value /= 5;
  }
  snafu.emplace_back(0);
  snafu.emplace_back(0);

  for (auto i = 0; i < snafu.size() - 1; ++i) {
    auto [c, corr] = correction.at(snafu[i]);
    snafu[i] = c;
    snafu[i + 1] += corr;
  }
  auto snafu_str = snafu | vw::reverse |
                   vw::drop_while([](auto v) { return v == 0; }) |
                   vw::transform([](auto c) { return dec_c[c]; }) |
                   vw::common;
  return std::string{snafu_str.begin(), snafu_str.end()};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto snafu = v.front().snafu;

  auto values = snafu | vw::transform(snafu_to_long) | vw::common;
  auto sum = std::accumulate(values.begin(), values.end(), 0L);

  return AOC_Output{long_to_snafu(sum)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{"Happy AoC day!"};
}

#include "../exec.hpp"