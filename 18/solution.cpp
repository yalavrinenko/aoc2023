//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <queue>
#include <set>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  long x, y, z;
  bool operator<(AOC_Input const &rhs) const {
    if (x == rhs.x) {
      if (y == rhs.y) {
        return z < rhs.z;
      } else {
        return y < rhs.y;
      }
    } else
      return x < rhs.x;
  }
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](auto const &s) {
           AOC_Input in{};
           std::sscanf(s.c_str(), "%ld,%ld,%ld", &in.x, &in.y, &in.z);
           in.x += 1;
           in.y += 1;
           in.z += 1;
           return in;
         }) |
         as_vector;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  constexpr long N = 40;
  std::array<std::array<std::array<char, N>, N>, N> map{};

  for (auto e: v) map[e.x][e.y][e.z] = '#';

  auto neight_count = [&map, N](auto i, auto j, auto k) {
    char count = 0;
    for (auto sh: {-1, 1}) {
      count +=
          (i + sh >= 0 && i + sh < N) ? map[i + sh][j][k] == '#' : 0;
      count +=
          (j + sh >= 0 && j + sh < N) ? map[i][j + sh][k] == '#' : 0;
      count +=
          (k + sh >= 0 && k + sh < N) ? map[i][j][k + sh] == '#' : 0;
    }
    return count;
  };

  auto count = 0L;
  for (auto i = 0; i < N; ++i)
    for (auto j = 0; j < N; ++j)
      for (auto k = 0; k < N; ++k) {
        if (map[i][j][k] != '#') {
          map[i][j][k] = neight_count(i, j, k);
          count += map[i][j][k];
        }
      }

  return AOC_Output{count};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  constexpr long N = 40;
  std::array<std::array<std::array<char, N>, N>, N> map{};

  for (auto e: v) map[e.x][e.y][e.z] = '#';

  auto neight_count = [&map, N](auto i, auto j, auto k) {
    char count = 0;
    for (auto sh: {-1, 1}) {
      count +=
          (i + sh >= 0 && i + sh < N) ? map[i + sh][j][k] == '#' : 0;
      count +=
          (j + sh >= 0 && j + sh < N) ? map[i][j + sh][k] == '#' : 0;
      count +=
          (k + sh >= 0 && k + sh < N) ? map[i][j][k + sh] == '#' : 0;
    }
    return count;
  };

  auto count = 0L;

  std::set<AOC_Input> visited;
  std::queue<AOC_Input> q;

  visited.insert(AOC_Input{0, 0, 0});
  q.push(AOC_Input{0, 0, 0});
  while (!q.empty()) {
    auto [x, y, z] = q.front();
    q.pop();

    count += neight_count(x, y, z);

    auto check_and_add = [&map, N, &visited, &q](auto x, auto y,
                                                 auto z) {
      if (x >= 0 && x < N && y >= 0 && y < N && z >= 0 && z < N &&
          map[x][y][z] != '#' &&
          !visited.contains(AOC_Input{x, y, z})) {
        visited.emplace(AOC_Input{x, y, z});
        q.push(AOC_Input{x, y, z});
      }
    };

    for (auto sh: {-1, 1}) {
      check_and_add(x + sh, y, z);
      check_and_add(x, y + sh, z);
      check_and_add(x, y, z + sh);
    }
  }

  return AOC_Output{count};
}

#include "../exec.hpp"