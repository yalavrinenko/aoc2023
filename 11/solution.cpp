//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <map>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

using Map = std::vector<std::string>;
struct AOC_Input {
  Map map;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines}};
}

struct point {
  int64_t i, j;
};

std::vector<point> extract_galaxy(Map const &map) {
  std::vector<point> galaxies;
  for (auto i = 0UL; i < map.size(); ++i)
    for (auto j = 0UL; j < map[i].size(); ++j) {
      if (map[i][j] == '#') { galaxies.emplace_back(i, j); }
    }
  return galaxies;
}

std::map<uint64_t, uint64_t> col_expansion(Map const &map,
                                           uint64_t expansion_step) {
  std::map<uint64_t, uint64_t> expansion{{0, 0}};
  auto expance = expansion_step;

  auto check_row = [&map](auto j) {
    for (const auto &i: map) {
      if (i[j] == '#') { return false; }
    }
    return true;
  };

  for (auto i = 0UL; i < map.front().size(); ++i) {
    if (check_row(i)) {
      expansion[i] = expance;
      expance += expansion_step;
    }
  }
  return expansion;
}

std::map<uint64_t, uint64_t> row_expansion(Map const &map,
                                           uint64_t expansion_step) {
  std::map<uint64_t, uint64_t> expansion{{0, 0}};
  auto expance = expansion_step;

  std::string golden(map.front().size(), '.');

  for (auto i = 0UL; i < map.size(); ++i) {
    if (map[i] == golden) {
      expansion[i] = expance;
      expance += expansion_step;
    }
  }
  return expansion;
}

void correct_galaxies(std::vector<point> &galaxies,
                      std::map<uint64_t, uint64_t> const &cols,
                      std::map<uint64_t, uint64_t> const &rows) {
  auto neares_lower = [](std::map<uint64_t, uint64_t> const &c,
                         auto v) {
    auto it = c.begin();
    while (it != c.end() && it->first < v) { ++it; }
    return (it == c.begin()) ? it : --it;
  };

  for (auto &g: galaxies) {
    auto cf = neares_lower(cols, g.j);
    g.j += cf->second;
    cf = neares_lower(rows, g.i);
    g.i += cf->second;
  }
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto galaxies = extract_galaxy(v.front().map);
  auto col_correction = col_expansion(v.front().map, 1);
  auto row_correction = row_expansion(v.front().map, 1);
  correct_galaxies(galaxies, col_correction, row_correction);
  uint64_t dist = 0;
  for (auto i = 0; i < galaxies.size() - 1; ++i) {
    for (auto j = i + 1; j < galaxies.size(); ++j) {
      dist += std::abs(galaxies[i].i - galaxies[j].i) +
              std::abs(galaxies[i].j - galaxies[j].j);
    }
  }
  return AOC_Output{dist};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto galaxies = extract_galaxy(v.front().map);
  auto col_correction = col_expansion(v.front().map, 1000000 - 1);
  auto row_correction = row_expansion(v.front().map, 1000000 - 1);
  correct_galaxies(galaxies, col_correction, row_correction);
  uint64_t dist = 0;
  for (auto i = 0; i < galaxies.size() - 1; ++i) {
    for (auto j = i + 1; j < galaxies.size(); ++j) {
      dist += std::abs(galaxies[i].i - galaxies[j].i) +
              std::abs(galaxies[i].j - galaxies[j].j);
    }
  }
  return AOC_Output{dist};
}

#include "../exec.hpp"