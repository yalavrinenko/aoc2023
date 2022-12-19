//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <deque>

struct AOC_Output {
  size_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string commands;
};

inline std::array blocks{
    std::vector{std::pair{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    std::vector{std::pair{0, 1}, {-1, 0}, {-1, 1}, {-1, 2}, {-2, 1}},
    std::vector{std::pair{0, 2}, {-1, 2}, {-2, 0}, {-2, 1}, {-2, 2}},
    std::vector{std::pair{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}},
    std::vector{std::pair{0, 0}, {0, 1}, {-1, 0}, {-1, 1}}};
inline std::array upshift = {1, 3, 3, 4, 2};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines.front()}};
}

auto tetris_mf(auto v, auto iters) {
  std::deque<std::string> box{
      "---------",
  };
  for ([[maybe_unused]] auto i: {0, 1, 2, 3})
    box.emplace_back("-.......-");

  size_t real_top = 0;
  auto top = 0;
  auto iter = 0;
  auto current_figure = 0;

  auto check_space = [&current_figure, &box](auto x, auto y) {
    for (auto shift: blocks[current_figure])
      if (box[x + shift.first][y + shift.second] != '.') return false;
    return true;
  };

  auto apply_shift = [line = v.front().commands](auto i, auto pos) {
    if (line[i] == '>') return std::pair{pos.first, pos.second + 1};
    else
      return std::pair{pos.first, pos.second - 1};
  };

  auto instr = 0;
  while (iter < iters) {
    auto start_pos = std::pair{top + 3 + upshift[current_figure], 3};

    bool stop_reached = false;
    while (!stop_reached) {
      auto try_move = apply_shift(instr, start_pos);
      if (check_space(try_move.first, try_move.second))
        start_pos = try_move;

      auto move_down =
          std::pair{start_pos.first - 1, start_pos.second};
      if (check_space(move_down.first, move_down.second))
        start_pos = move_down;
      else {
        if (top < start_pos.first) {
          real_top += start_pos.first - top;
          top = start_pos.first;
        }

        for (auto shift: blocks[current_figure])
          box[start_pos.first + shift.first]
             [start_pos.second + shift.second] = '#';
        stop_reached = true;
      }

      instr = (instr + 1) % v.front().commands.size();
    }

    while (box.size() <= top + 10) { box.emplace_back("-.......-"); }

    current_figure = (current_figure + 1) % blocks.size();
    ++iter;
  }
  fmt::print("==============={}=================\n", top);
  for (auto line: box | vw::reverse) fmt::print("{}\n", line);
  return real_top;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return AOC_Output{tetris_mf(v, 2022)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{tetris_mf(v, 2022)};
}

#include "../exec.hpp"