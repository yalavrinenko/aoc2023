//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <format>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::pair<long, long>> barrier;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &scan) {
           AOC_Input in;
           in.barrier = scan | vw::lazy_split(std::string{" -> "}) |
                        vw::transform([](const auto &pstr) {
                          std::istringstream iss{
                              fmt::format("{}", fmt::join(pstr, ""))};
                          std::pair<long, long> point;
                          iss >> point.second;
                          iss.ignore();
                          iss >> point.first;
                          return point;
                        }) |
                        as_vector;
           return in;
         }) |
         as_vector;
}

auto make_map(const auto &v) {
  std::array<std::array<char, 1000>, 200> map{{}};

  auto plot_line = [&map](const auto &start, auto vec) {
    auto &[vx, vy] = vec;
    while (vx != 0 || vy != 0) {
      map[start.first + vx][start.second + vy] = '#';
      if (vx != 0) vx += (vx < 0) ? 1 : -1;
      if (vy != 0) vy += (vy < 0) ? 1 : -1;
    }
    map[start.first][start.second] = '#';
  };

  long lowest_bound = 0;

  auto plot_rock = [&plot_line, &lowest_bound](
                       const std::vector<std::pair<long, long>> &in) {
    if (lowest_bound < in[0].first) lowest_bound = in[0].first;

    for (auto i = 1; i < in.size(); ++i) {
      plot_line(in[i - 1],
                std::pair(in[i].first - in[i - 1].first,
                          in[i].second - in[i - 1].second));
      if (lowest_bound < in[i].first) lowest_bound = in[i].first;
    }
  };

  std::ranges::for_each(v, plot_rock, &AOC_Input::barrier);

  return std::pair{map, lowest_bound};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto [map, low_bound] = make_map(v);

  auto trace_sand = [&map, low_bound]() {
    long x = 0, y = 500;
    bool is_stop = false;
    while (!is_stop) {
      if (map[x + 1][y] == 0) ++x;
      else if (map[x + 1][y - 1] == 0) {
        ++x;
        --y;
      } else if (map[x + 1][y + 1] == 0) {
        ++x;
        ++y;
      } else {
        map[x][y] = 'o';
        is_stop = true;
      }

      if (x >= low_bound) return true;
    }
    return false;
  };

  auto sand_count = 0;
  while (!trace_sand()) { ++sand_count; }
  
  return AOC_Output{sand_count};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto [map, low_bound] = make_map(v);

  for (auto i = 0; i < map[0].size(); ++i)
    map[low_bound + 2][i] = '#';

  auto trace_sand = [&map]() {
    long x = 0, y = 500;
    bool is_stop = false;
    while (!is_stop) {
      if (map[x + 1][y] == 0) ++x;
      else if (map[x + 1][y - 1] == 0) {
        ++x;
        --y;
      } else if (map[x + 1][y + 1] == 0) {
        ++x;
        ++y;
      } else {
        map[x][y] = 'o';
        is_stop = true;
      }
    }
    return (x == 0) && (y == 500);
  };

  auto sand_count = 0;
  while (!trace_sand()) { ++sand_count; }

  return AOC_Output{sand_count + 1};
}

#include "../exec.hpp"