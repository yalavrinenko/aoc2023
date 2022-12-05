//
// Created by yalavrinenko on 02.12.2020.
//

#ifndef AOC_2020_MAIN_HPP
#define AOC_2020_MAIN_HPP
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include <algorithm>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <unordered_map>

namespace vw = std::ranges::views;


struct AOC_Input;
struct AOC_Output;

AOC_Output part_1(std::vector<AOC_Input> const &in);
AOC_Output part_2(std::vector<AOC_Input> const &in);

struct AOC_Reader {
  static std::vector<AOC_Input>
  from_string_vector(const std::vector<std::string> &line);
};

template<typename V>
auto to_vector(V &&v) {
  decltype(auto) cv = std::forward<V>(v) | vw::common;
  return std::vector(cv.begin(), cv.end());
}

struct as_vector_impl {};

inline auto as_vector = as_vector_impl{};

template<typename V>
inline auto operator|(V &&v, const as_vector_impl &obj) {
  return to_vector(std::forward<V>(v));
}

template<typename T>
struct srange_to_value {
  static auto operator()(auto &&r) {
    T init{};
    for (auto e: r) {
      init = init * 10 + e - '0';
    }
    return init;
  }
};

#endif//AOC_2020_MAIN_HPP
