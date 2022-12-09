//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <set>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  char direction;
  int steps;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &e) {
           std::istringstream iss{e};
           char dir;
           int steps;
           iss >> dir >> steps;
           return AOC_Input{dir, steps};
         }) |
         as_vector;
}

auto move_tail(auto tail, auto head) {
  auto [tx, ty] = tail;
  auto [hx, hy] = head;
  if ((hx - tx) * (hx - tx) + (hy - ty) * (hy - ty) > 2) {
    auto vx = hx - tx;
    auto vy = hy - ty;
    if (vx != 0) vx /= std::abs(vx);
    if (vy != 0) vy /= std::abs(vy);
    return std::pair{tx + vx, ty + vy};
  }
  return tail;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  std::set<std::pair<int, int>> tail_position_map{{0, 0}};

  std::pair head_position{0, 0};
  std::pair tail_position{0, 0};
  std::ranges::for_each(v, [&tail_position, &tail_position_map,
                            &head_position](const auto &e) {
    for (auto i = 0; i < e.steps; ++i) {
      switch (e.direction) {
        case 'L':
          --head_position.first;
          break;
        case 'R':
          ++head_position.first;
          break;
        case 'D':
          --head_position.second;
          break;
        case 'U':
          ++head_position.second;
          break;
      }
      tail_position = move_tail(tail_position, head_position);
      tail_position_map.emplace(tail_position);
    }
  });

  return AOC_Output{tail_position_map.size()};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::set<std::pair<int, int>> nine_tail_position_map{{0, 0}};

  std::array<std::pair<int, int>, 10> knots;
  knots.fill(std::pair{0, 0});

  std::ranges::for_each(
      v, [&knots, &nine_tail_position_map](const auto &e) {
        for (auto i = 0; i < e.steps; ++i) {
          switch (e.direction) {
            case 'L':
              --knots[0].first;
              break;
            case 'R':
              ++knots[0].first;
              break;
            case 'D':
              --knots[0].second;
              break;
            case 'U':
              ++knots[0].second;
              break;
          }
          for (auto k = 1; k < knots.size(); ++k)
            knots[k] = move_tail(knots[k], knots[k - 1]);
          nine_tail_position_map.emplace(knots[9]);
        }
      });

  return AOC_Output{nine_tail_position_map.size()};
}

#include "../exec.hpp"