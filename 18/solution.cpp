//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <map>
#include <set>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  char D;
  int count;
  std::string color;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &s) {
           std::istringstream iss{s};
           AOC_Input in;
           iss >> in.D >> in.count >> in.color;
           return in;
         }) |
         as_vector;
}

std::unordered_map<char, std::pair<int, int>> C2D{{'U', {0, -1}},
                                                  {'D', {0, 1}},
                                                  {'L', {-1, 0}},
                                                  {'R', {1, 0}}};

std::unordered_map<char, std::pair<int, int>> I2D{{'3', {0, -1}},
                                                  {'1', {0, 1}},
                                                  {'2', {-1, 0}},
                                                  {'0', {1, 0}}};

using Map = std::map<int, std::set<int>>;

AOC_Output part_1(std::vector<AOC_Input> const &v) {

  std::pair<int, int> p{0, 0};

  std::pair<int, int> prev{0, 0};

  auto A = 0LL;
  auto P = 0LL;

  for (auto &cmd: v) {
    auto &S = C2D[cmd.D];
    p.first += S.first * (cmd.count);
    p.second += S.second * (cmd.count);
    A += (p.second * prev.first - p.first * prev.second);
    P += std::abs(p.second - prev.second) +
         std::abs(p.first - prev.first);
    prev = p;
  }

  return AOC_Output{A / 2 + P / 2 + 1};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::pair<long long, long long> p{0, 0};

  std::pair<long long, long long> prev{0, 0};

  auto A = 0LL;
  auto P = 0LL;

  for (auto cmd: v) {
    std::ranges::replace_if(
        cmd.color,
        [](char c) { return c == '(' || c == ')' || c == '#'; }, ' ');

    cmd.color.pop_back();
    auto &S = I2D[cmd.color.back()];
    cmd.color.pop_back();
    std::istringstream iss{cmd.color};
    auto count = 1;
    iss >> std::hex >> count;

    p.first += S.first * (count);
    p.second += S.second * (count);
    A += (p.second * prev.first - p.first * prev.second);
    P += std::abs(p.second - prev.second) +
         std::abs(p.first - prev.first);
    prev = p;
  }

  return AOC_Output{A / 2 + P / 2 + 1};
}

#include "../exec.hpp"