//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <cassert>
#include <queue>

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
  return {AOC_Input{lines}};
}

struct QEntry {
  long x, y;
  long steps = 0;
  auto operator==(const QEntry &rhs) const {
    return (x == rhs.x) && (y == rhs.y);
  }
};

std::optional<long> bfs(const auto &map, auto start, auto end) {
  auto visited = map;

  auto check = [&map, &visited](QEntry current, long i, long j) {
    if (i < 0 || j < 0 || i >= map.size() ||
        j >= map.front().size() || visited[i][j] == '.' ||
        map[i][j] - map[current.x][current.y] > 1)
      return false;
    return true;
  };

  static constexpr std::array<std::pair<int, int>, 4> shifts =
      {std::pair{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

  std::queue<QEntry> q;
  q.push(start);
  visited[start.x][start.y] = '.';
  while (!q.empty() && !(q.front() == end)) {
    auto e = q.front();
    q.pop();
    for (auto &[xs, ys]: shifts) {
      if (check(e, e.x + xs, e.y + ys)) {
        q.emplace(QEntry{e.x + xs, e.y + ys, e.steps + 1});
        visited[e.x + xs][e.y + ys] = '.';
      }
    }
  }

  if (q.empty()) return std::nullopt;
  return std::optional{q.front().steps};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto map = v.front().map;
  QEntry start, end;
  for (auto i = 0; i < map.size(); ++i)
    for (auto j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == 'S') {
        start = QEntry{i, j, 0};
        map[i][j] = 'a';
      }
      if (map[i][j] == 'E') {
        end = {i, j, 0};
        map[i][j] = 'z';
      }
    }

  return AOC_Output{bfs(map, start, end).value()};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto map = v.front().map;
  std::optional<long> opt_distance;
  QEntry end;
  for (auto i = 0; i < map.size(); ++i)
    for (auto j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == 'S') { map[i][j] = 'a'; }
      if (map[i][j] == 'E') {
        end = {i, j, 0};
        map[i][j] = 'z';
      }
    }

  for (auto i = 0; i < map.size(); ++i)
    for (auto j = 0; j < map[i].size(); ++j) {
      if (map[i][j] == 'a') {
        auto path = bfs(map, QEntry{i, j, 0}, end);
        if (path) {
          if (!opt_distance) opt_distance = *path;
          if (*path < *opt_distance) opt_distance = *path;
        }
      }
    }

  return AOC_Output{*opt_distance};
}

#include "../exec.hpp"