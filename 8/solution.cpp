//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  unsigned long value{};
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

auto tree_visibility_map(const auto &map) {
  std::vector<std::vector<char>> visibility_map(
      map.size(), std::vector<char>(map.front().size(), 0));

  for (std::weakly_incrementable auto i:
       vw::iota(1UL, map.size() - 1)) {
    auto max_value_fw = map[i][0];
    auto max_value_bw = map[i][map[i].size() - 1];
    visibility_map[i][0] = visibility_map[i][map[i].size() - 1] = 1;
    for (std::weakly_incrementable auto j:
         vw::iota(1UL, map.size() - 1)) {
      if (map[i][j] > max_value_fw) {
        max_value_fw = map[i][j];
        visibility_map[i][j] = 1;
      }
      if (map[i][map.size() - 1 - j] > max_value_bw) {
        max_value_bw = map[i][map.size() - 1 - j];
        visibility_map[i][map.size() - 1 - j] = 1;
      }
    }
  }

  for (std::weakly_incrementable auto i:
       vw::iota(1UL, map.front().size() - 1)) {
    auto rows = map.front().size();
    auto max_value_fw = map[0][i];
    auto max_value_bw = map[rows - 1][i];
    visibility_map[0][i] = visibility_map[rows - 1][i] = 1;
    for (std::weakly_incrementable auto j: vw::iota(1UL, rows - 1)) {
      if (map[j][i] > max_value_fw) {
        max_value_fw = map[j][i];
        visibility_map[j][i] = 1;
      }
      if (map[rows - 1 - j][i] > max_value_bw) {
        max_value_bw = map[rows - 1 - j][i];
        visibility_map[rows - 1 - j][i] = 1;
      }
    }
  }
  return visibility_map;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto &map = v.front().map;
  auto visibility_map = tree_visibility_map(map);
  return AOC_Output{
      std::accumulate(
          visibility_map.begin(), visibility_map.end(), 0UL,
          [](auto sum, const auto &v) {
            return sum + std::accumulate(v.begin(), v.end(), 0UL,
                                         std::plus{});
          }) +
      4};
}

auto view_score(const auto &map, auto x, auto y) {
  auto vleft = 0;
  for (int i = y - 1; i >= 0; --i) {
    if (map[x][i] < map[x][y]) ++vleft;
    else {
      ++vleft;
      break;
    }
  }

  auto vright = 0;
  for (int i = y + 1; i < map.front().size(); ++i) {
    if (map[x][i] < map[x][y]) ++vright;
    else {
      ++vright;
      break;
    }
  }

  auto vup = 0;
  for (int i = x - 1; i >= 0; --i) {
    if (map[i][y] < map[x][y]) ++vup;
    else {
      ++vup;
      break;
    }
  }

  auto vdown = 0;
  for (int i = x + 1; i < map.size(); ++i) {
    if (map[i][y] < map[x][y]) ++vdown;
    else {
      ++vdown;
      break;
    }
  }
  return vleft * vdown * vright * vup;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto &map = v.front().map;
  auto vmap = tree_visibility_map(map);

  auto max_score = 0UL;

  for (std::weakly_incrementable auto i: vw::iota(0UL, vmap.size()))
    for (std::weakly_incrementable auto j:
         vw::iota(0UL, vmap.front().size())) {
      if (vmap[i][j] == 1) {
        auto score = view_score(map, i, j);
        if (score > max_score) max_score = score;
      }
    }

  return AOC_Output{max_score};
}

#include "../exec.hpp"