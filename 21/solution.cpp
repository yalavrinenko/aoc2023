//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

#include <set>


#include <queue>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

using Map = std::vector<std::string>;
struct AOC_Input {
  Map m;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines}};
}

long bsf(Map m, int x, int y, int steps) {
  struct Bfs {
    int x, y;
    int step;
  };

  std::vector<std::vector<std::vector<bool>>> visited(
      m.size(),
      std::vector<std::vector<bool>>(
          m.front().size(), std::vector<bool>(steps, false)));

  std::queue<Bfs> Q;
  visited[x][y][0] = true;
  m[x][y] = '.';
  Q.emplace(Bfs{x, y, 0});

  std::vector<Bfs> final;

  while (!Q.empty()) {
    auto v = Q.front();
    Q.pop();

    if (v.step == steps) {
      final.emplace_back(v);
      continue;
    }

    for (auto [dx, dy]: {std::pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      auto nx = v.x + dx;
      auto ny = v.y + dy;
      auto step = v.step + 1;
      if (nx >= 0 && nx < m.size() && ny >= 0 &&
          ny < m.front().size()) {
        if (!visited[nx][ny][step] && m[nx][ny] != '#') {
          Q.emplace(Bfs{nx, ny, step});
          visited[nx][ny][step] = true;
        }
      }
    }
  }

  return final.size();
}


long bsf2(Map m, int x, int y, int steps) {
  struct Bfs {
    int x, y;
    int step;
    auto operator<(Bfs const &rhs) const {
      if (x == rhs.x) {
        if (y == rhs.y) { return step < rhs.step; }
        return y < rhs.y;
      }
      return x < rhs.x;
    }
  };

  std::set<Bfs> visited;

  std::queue<Bfs> Q;
  m[x][y] = '.';
  Q.emplace(Bfs{x, y, 0});
  visited.insert(Bfs{x, y, 0});

  std::vector<Bfs> final;

  auto wrapped_access = [&m](long nx, long ny) {
    if (nx >= m.size()) nx = nx % m.size();
    if (ny >= m.front().size()) ny = ny % m.size();
    if (nx < 0) { nx = m.size() - 1 - (nx % m.size()); }
    if (ny < 0) {
      ny = m.front().size() - 1 - (ny % m.front().size());
    }
    return m[nx][ny];
  };

  while (!Q.empty()) {
    auto v = Q.front();
    Q.pop();

    if (v.step == steps) {
      final.emplace_back(v);
      continue;
    }

    for (auto [dx, dy]: {std::pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      auto nx = v.x + dx;
      auto ny = v.y + dy;
      auto step = v.step + 1;
      Bfs next{nx, ny, step};
      if (!visited.contains(next) && wrapped_access(nx, ny) != '#') {
        Q.emplace(next);
        visited.insert(next);
      }
    }
  }

  return final.size();
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  int sx, sy;
  auto &m = v.front().m;
  for (auto i = 0; i < v.front().m.size(); ++i)
    for (auto j = 0; j < v.front().m[i].size(); ++j) {
      if (m[i][j] == 'S') {
        sx = i;
        sy = j;
      }
    }

  return AOC_Output{bsf(m, sx, sy, 64)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  int sx, sy;
  auto &m = v.front().m;
  for (auto i = 0; i < v.front().m.size(); ++i)
    for (auto j = 0; j < v.front().m[i].size(); ++j) {
      if (m[i][j] == 'S') {
        sx = i;
        sy = j;
      }
    }

  return AOC_Output{bsf2(m, sx, sy, 26501365)};
}

#include "../exec.hpp"