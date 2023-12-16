//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <unordered_set>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::string> map;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines}};
}

auto move_rocks_row(std::vector<std::string> &s, int axis) {
  auto base = 0;
  for (auto i = 0; i < s.size(); ++i) {
    if (s[i][axis] == '#') { base = i + 1; }
    if (s[i][axis] == 'O') {
      if (i == base) {
        ++base;
      } else {
        s[base][axis] = 'O';
        s[i][axis] = '.';
        base++;
      }
    }
  }
}

auto move_rock(std::vector<std::string> &map) {
  for (auto i = 0; i < map.front().size(); ++i) {
    move_rocks_row(map, i);
  }
}

auto hash(std::vector<std::string> const &map) {
  auto sum = 0ULL;
  auto scale = map.size();
  for (auto &line: map) {
    sum += std::ranges::count(line, 'O') * scale;
    --scale;
  }
  return sum;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto map = v.front().map;
  move_rock(map);
  return AOC_Output{hash(map)};
}

auto rotate_map(std::vector<std::string> const &m) {
  std::vector<std::string> rotated(m.front().size());
  for (auto i = 0; i < m.front().size(); ++i) {
    for (auto j = 0; j < m.size(); ++j) { rotated[i] += m[j][i]; }
    std::reverse(rotated[i].begin(), rotated[i].end());
  }
  return rotated;
}


AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto map = v.front().map;
  static constexpr auto cycles = 118;

  for (auto i = 0; i < cycles; ++i) {
    move_rock(map);

    map = rotate_map(map);
    move_rock(map);

    map = rotate_map(map);
    move_rock(map);

    map = rotate_map(map);
    move_rock(map);

    map = rotate_map(map);
  }
  return AOC_Output{hash(map)};
}

#include "../exec.hpp"