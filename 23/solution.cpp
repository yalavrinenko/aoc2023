//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <map>
#include <queue>
#include <set>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::set<std::pair<long, long>> map;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  AOC_Input in;
  for (auto i = 0; i < lines.size(); ++i)
    for (auto j = 0; j < lines[i].size(); ++j)
      if (lines[i][j] == '#') in.map.emplace(i, j);
  return {in};
}

struct move {
  std::array<std::pair<long, long>, 3> checks;
  std::pair<long, long> step;
};

inline constexpr std::array moves{
    move{{std::pair{-1, -1}, {-1, 0}, {-1, 1}}, {-1, 0}},
    move{{std::pair{1, -1}, {1, 0}, {1, 1}}, {1, 0}},
    move{{std::pair{-1, -1}, {0, -1}, {1, -1}}, {0, -1}},
    move{{std::pair{-1, 1}, {0, 1}, {1, 1}}, {0, 1}},
};

auto round(auto &order, auto &map) {
  auto snap = map;

  auto check_move = [](auto &map, const move &m,
                       std::pair<long, long> c) {
    for (auto [dx, dy]: m.checks)
      if (map.contains(std::pair{c.first + dx, c.second + dy}))
        return false;
    return true;
  };

  std::map<std::pair<long, long>, std::vector<std::pair<long, long>>>
      next;

  for (auto elf: map) {
    bool should_move = false;
    for (auto i: {-1, 0, 1})
      for (auto j: {-1, 0, 1}) {
        if (i == 0 && j == 0) continue;
        if (map.contains(std::pair{elf.first + i, elf.second + j}))
          should_move = true;
      }

    if (should_move) {
      auto proposal = elf;
      for (auto check: order) {
        if (check_move(map, moves[check], elf)) {
          proposal = std::pair{elf.first + moves[check].step.first,
                               elf.second + moves[check].step.second};
          break;
        }
      }
      next[proposal].emplace_back(elf);
    } else
      next[elf].emplace_back(elf);
  }

  auto size_eta = map.size();

  map.clear();
  for (auto &kv: next) {
    if (kv.second.size() == 1) map.emplace(kv.first);
    else {
      for (auto ef: kv.second) map.emplace(ef);
    }
  }

  if (size_eta != map.size()) throw "Fuuuck!";

  order.push_back(order.front());
  order.pop_front();

  return map == snap;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  std::deque<long> order{0, 1, 2, 3};
  auto map = v.front().map;

  for (auto step = 0; step < 10; ++step) { round(order, map); }

  auto [min_x, max_x] = std::ranges::minmax(
      map, std::less{}, &std::pair<long, long>::first);

  auto [min_y, max_y] = std::ranges::minmax(
      map, std::less{}, &std::pair<long, long>::second);

  long res = (max_x.first - min_x.first + 1) *
                 (max_y.second - min_y.second + 1) -
             map.size();

  return AOC_Output{res};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::deque<long> order{0, 1, 2, 3};
  auto map = v.front().map;

  auto step = 1;
  while (!round(order, map)) { ++step; }

  return AOC_Output{step};
}

#include "../exec.hpp"