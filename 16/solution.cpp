//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <queue>

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

using map = std::vector<std::string>;

auto trace_beam(map const &m, int x, int y,
                std::pair<int, int> idir) {
  struct beam {
    int x, y;
    std::pair<int, int> dir;
  };
  std::queue<beam> Q;

  std::vector<std::vector<std::vector<std::pair<int, int>>>> visited(
      m.size(), std::vector<std::vector<std::pair<int, int>>>(
                    m.front().size()));

  if (m[y][x] == '.') {
    Q.emplace(beam{.x = x, .y = y, .dir = idir});
  }

  if (m[y][x] == '|') {
    if (idir.first == 0) {
      Q.emplace(beam{.x = x, .y = y, .dir = idir});
    } else {
      Q.emplace(beam{.x = x, .y = y, .dir = {0, 1}});
      Q.emplace(beam{.x = x, .y = y, .dir = {0, -1}});
    }
  }

  if (m[y][x] == '-') {
    if (idir.second == 0) {
      Q.emplace(beam{.x = x, .y = y, .dir = idir});
    } else {
      Q.emplace(beam{.x = x, .y = y, .dir = {1, 0}});
      Q.emplace(beam{.x = x, .y = y, .dir = {-1, 0}});
    }
  }

  if (m[y][x] == '/') {
    Q.emplace(
        beam{.x = x, .y = y, .dir = {-idir.second, -idir.first}});
  }

  if (m[y][x] == '\\') {
    Q.emplace(beam{.x = x, .y = y, .dir = {idir.second, idir.first}});
  }

  auto count = 0ULL;

  while (!Q.empty()) {
    auto b = Q.front();
    Q.pop();

    if (visited[b.y][b.x].empty()) { ++count; }

    if (std::count(visited[b.y][b.x].begin(), visited[b.y][b.x].end(),
                   b.dir)) {
      continue;
    } else {
      visited[b.y][b.x].emplace_back(b.dir);
    }

    auto nx = b.x + b.dir.first;
    auto ny = b.y + b.dir.second;

    if (nx < 0 || nx >= m.front().size() || ny < 0 || ny >= m.size())
      continue;

    if (m[ny][nx] == '.') {
      Q.emplace(beam{.x = nx, .y = ny, .dir = b.dir});
    }

    if (m[ny][nx] == '|') {
      if (b.dir.first == 0) {
        Q.emplace(beam{.x = nx, .y = ny, .dir = b.dir});
      } else {
        Q.emplace(beam{.x = nx, .y = ny, .dir = {0, 1}});
        Q.emplace(beam{.x = nx, .y = ny, .dir = {0, -1}});
      }
    }

    if (m[ny][nx] == '-') {
      if (b.dir.second == 0) {
        Q.emplace(beam{.x = nx, .y = ny, .dir = b.dir});
      } else {
        Q.emplace(beam{.x = nx, .y = ny, .dir = {1, 0}});
        Q.emplace(beam{.x = nx, .y = ny, .dir = {-1, 0}});
      }
    }

    if (m[ny][nx] == '/') {
      Q.emplace(beam{.x = nx,
                     .y = ny,
                     .dir = {-b.dir.second, -b.dir.first}});
    }

    if (m[ny][nx] == '\\') {
      Q.emplace(
          beam{.x = nx, .y = ny, .dir = {b.dir.second, b.dir.first}});
    }
  }
  return count;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto &m = v.front().map;
  return AOC_Output{trace_beam(m, 0, 0, {1, 0})};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto &m = v.front().map;
  auto max_E = 0ULL;
  for (auto i = 0; i < m.size(); ++i) {
    max_E = std::max(max_E, trace_beam(m, 0, i, {1, 0}));
    max_E = std::max(max_E,
                     trace_beam(m, m.front().size() - 1, i, {-1, 0}));
  }

  for (auto i = 0; i < m.front().size(); ++i) {
    max_E = std::max(max_E, trace_beam(m, i, 0, {0, 1}));
    max_E = std::max(max_E, trace_beam(m, i, m.size() - 1, {0, -1}));
  }

  return AOC_Output{max_E};
}

#include "../exec.hpp"