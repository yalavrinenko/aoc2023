//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<long> food_c;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  auto inp = lines | vw::lazy_split(std::string{}) |
             vw::transform([](auto inner_v) {
               AOC_Input in;
               auto v = inner_v | vw::transform([](auto const &line) {
                          return std::stol(line);
                        }) |
                        vw::common;
               in.food_c = std::vector(v.begin(), v.end());
               return in;
             }) |
             vw::common;
  return {inp.begin(), inp.end()};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return std::ranges::max(v | vw::transform([](const auto &e) {
                            return std::accumulate(
                                e.food_c.begin(), e.food_c.end(), 0L);
                          }));
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto top_elfs_v =
      v | vw::transform([](const auto &e) {
        return std::accumulate(e.food_c.begin(), e.food_c.end(), 0L);
      }) |
      vw::common;
  auto top_elfs = std::vector(top_elfs_v.begin(), top_elfs_v.end());
  std::ranges::sort(top_elfs, std::greater{});
  return std::reduce(top_elfs.begin(), top_elfs.begin() + 3, 0L);
}

#include "../exec.hpp"