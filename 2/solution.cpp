//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <unordered_map>

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  char P1, P2;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &line) {
           return AOC_Input{line[0],
                            static_cast<char>(line[2] - ('X' - 'A'))};
         }) |
         as_vector;
}

inline auto win_map =
    std::unordered_map{std::pair{'A', 'C'}, {'B', 'A'}, {'C', 'B'}};

inline auto loose_map =
    std::unordered_map{std::pair{'A', 'B'}, {'B', 'C'}, {'C', 'A'}};

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto sum_func = [](auto sum, const auto &e) {
    auto win_loose_score =
        (e.P1 == e.P2) ? 3 : (win_map[e.P2] == e.P1 ? 6 : 0);
    return sum + (e.P2 - 'A' + 1) + win_loose_score;
  };
  return std::accumulate(v.begin(), v.end(), 0, sum_func);
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto sum_func = [](auto sum, const auto &e) {
    auto win_loose_score = (e.P2 - 'A') * 3;
    auto your_turn = (win_loose_score == 3)
                         ? e.P1
                         : ((win_loose_score == 0) ? win_map[e.P1]
                                                   : loose_map[e.P1]);
    return sum + (your_turn - 'A' + 1) + win_loose_score;
  };
  return std::accumulate(v.begin(), v.end(), 0, sum_func);
}

#include "../exec.hpp"