//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output{
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator() () const { return value; }
};

struct AOC_Input{
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(const std::vector<std::string> &lines) {
  return {};
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  return AOC_Output{};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  return AOC_Output{};
}

#include "../exec.hpp"