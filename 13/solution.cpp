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
  std::vector<std::string> map;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  auto maps = lines | vw::split("") |
              vw::transform([](const auto &map) {
                return AOC_Input{map | as_vector};
              }) |
              as_vector;
  return maps;
}

auto find_horizontal_reflection(std::vector<std::string> const &map) {
  auto check_reflection = [&map](int center) {
    auto up = center;
    auto down = center - 1;
    while (up < map.size() && down >= 0) {
      if (map[up] != map[down]) return false;
      ++up;
      --down;
    }
    return true;
  };
  for (auto i = 1; i < map.size(); ++i) {
    if (check_reflection(i)) return i;
  }
  return -1;
}

auto find_vertical_reflection(std::vector<std::string> const &map) {
  auto cmp_row = [&map](int lhs, int rhs) {
    for (auto &line: map) {
      if (line[lhs] != line[rhs]) return false;
    }
    return true;
  };

  auto check_reflection = [&map, &cmp_row](int center) {
    auto right = center;
    auto left = center - 1;
    while (right < map.front().size() && left >= 0) {
      if (!cmp_row(left, right)) return false;
      ++right;
      --left;
    }
    return true;
  };
  for (auto i = 1; i < map.front().size(); ++i) {
    if (check_reflection(i)) return i;
  }
  return -1;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {

  auto rf = v | vw::transform([](const auto &e) {
              return std::pair{find_horizontal_reflection(e.map),
                               find_vertical_reflection(e.map)};
            });

  return AOC_Output{std::accumulate(rf.begin(), rf.end(), 0LL,
                                    [](auto sum, const auto &p) {
                                      if (p.first != -1) {
                                        return sum + p.first * 100;
                                      }
                                      return sum + p.second;
                                    })};
}

auto shake(std::vector<std::string> map, std::pair<int, int> origin) {
  for (auto i = 0; i < map.size(); ++i)
    for (auto j = 0; j < map[i].size(); ++j) {
      map[i][j] = (map[i][j] == '#') ? '.' : '#';

      auto nref = std::pair{find_horizontal_reflection(map),
                            find_vertical_reflection(map)};

      if (nref != std::pair{-1, -1} && nref != origin) {
        return (nref.first != -1) ? nref.first * 100 : nref.second;
      }

      map[i][j] = (map[i][j] == '#') ? '.' : '#';
    }

  throw "Fuck";
}

int countDifferingCharacters(const std::string &str1,
                             const std::string &str2) {
  int count = 0;
  for (int i = 0; i < str1.length(); i++) {
    if (str1[i] != str2[i]) { count++; }
  }

  return count;
}

int findMirror(std::vector<std::string> &group) {
  for (int split = 1; split < group.size(); split++) {
    int size = std::min(split, (int) group.size() - split);

    int smudgeCount = 0;
    for (int y = 0; y < size; y++) {
      std::string side1 = group[split - 1 - y];
      std::string side2 = group[split + y];
      smudgeCount += countDifferingCharacters(side1, side2);
    }
    if (smudgeCount == 1) { return split; }
  }
  return 0;
}

std::vector<std::string> rotate(std::vector<std::string> &group) {
  std::vector<std::string> ret;

  int width = group[0].length();
  for (int i = 0; i < width; i++) {
    std::string s = "";

    for (int j = 0; j < group.size(); j++) {
      s += group[group.size() - j - 1][i];
    }

    ret.push_back(s);
  }

  return ret;
}

uint64_t getScore(std::vector<std::string> &group) {
  uint64_t ret = findMirror(group) * 100;
  std::vector<std::string> rotated = rotate(group);
  ret += findMirror(rotated);

  return ret;
}


AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto sum = 0;
  for (auto e: v) { sum += getScore(e.map); }
  return AOC_Output{sum};
}

#include "../exec.hpp"