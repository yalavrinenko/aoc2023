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
  std::pair<std::vector<long>, std::vector<long>> pads;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &in) {
           auto pads =
               in | vw::lazy_split(',') |
               vw::transform([](const auto &e) {
                 return e | vw::lazy_split('-') |
                        vw::transform(srange_to_value<long>{}) |
                        as_vector;
               });
           return AOC_Input{
               std::pair{pads.front(), (pads | vw::drop(1)).front()}};
         }) |
         as_vector;
}

auto is_contains(const auto &lhs, const auto &rhs) {
  return lhs[0] <= rhs[0] && rhs[1] <= lhs[1];
}

auto is_overlap(const auto &lhs, const auto &rhs) {
  return (lhs[0] <= rhs[0] && rhs[0] <= lhs[1]) ||
         (rhs[0] <= lhs[0] && lhs[0] <= rhs[1]);
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return AOC_Output{std::accumulate(
      v.begin(), v.end(), 0L, [](auto sum, const auto &e) {
        return sum + (is_contains(e.pads.first, e.pads.second) ||
                      is_contains(e.pads.second, e.pads.first));
      })};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{std::accumulate(
      v.begin(), v.end(), 0L, [](auto sum, const auto &e) {
        return sum + is_overlap(e.pads.first, e.pads.second);
      })};
}

#include "../exec.hpp"