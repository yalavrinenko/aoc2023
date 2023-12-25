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

using Map = std::vector<std::string>;
struct AOC_Input {
  Map m;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines}};
}

long longest_path(Map const &m) {
  struct Vertex {
    long x, y, steps;
    std::set<std::pair<long, long>> path;
  };

  std::queue<Vertex> Q;
  Q.emplace(Vertex{0, 1, 0, {std::pair<long, long>{0, 1}}});

  long max_steps = 0;

  while (!Q.empty()) {
    auto v = Q.front();
    Q.pop();

    if (v.x == m.size() - 1 && v.y == m.front().size() - 2) {
      max_steps = std::max(max_steps, v.steps);
      continue;
    }

    for (auto [dx, dy]: {std::pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      auto nx = v.x + dx;
      auto ny = v.y + dy;
      auto steps = v.steps + 1;

      if (nx >= 0 && nx < m.size() && ny >= 0 &&
          ny < m.front().size()) {

        if (m[nx][ny] == '>') {
          ny += 1;
          steps += 1;
        }

        if (m[nx][ny] == 'v') {
          nx += 1;
          steps += 1;
        }

        if (m[nx][ny] != '#' && !v.path.contains(std::pair{nx, ny})) {
          Vertex nv{nx, ny, steps, v.path};
          nv.path.insert(std::pair{nx, ny});
          Q.emplace(std::move(nv));
        }
      }
    }
  }

  return max_steps;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  //return AOC_Output{0};
  return AOC_Output{longest_path(v.front().m)};
}

long max_v = 0;
long longest_path_v2(Map const &m, long x, long y,
                     std::vector<std::vector<int>> &path) {

  if (x == m.size() - 1 && y == m.front().size() - 2) {
    auto count = 0;
    for (auto i = 0; i < m.size(); ++i)
      for (auto j = 0; j < m.front().size(); ++j) {
        //fmt::print("{}", (path[i][j]) ? 'O' : m[i][j]);
        count += path[i][j];
      }

    if (max_v < count) {
      fmt::print("{}\n", count);
      max_v = count;
    }
    return count;
  }

  long max_steps = 0;

  for (auto [dx, dy]: {std::pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
    auto nx = x + dx;
    auto ny = y + dy;

    if (m[nx][ny] != '#' && !path[nx][ny]) {
      path[nx][ny] = true;
      max_steps =
          std::max(longest_path_v2(m, nx, ny, path), max_steps);
      path[nx][ny] = false;
    }
  }

  return max_steps;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  // auto m = v.front().m;
  // m[0][1] = '#';
  // std::vector<std::vector<int>> path(
  //     m.size(), std::vector<int>(m.front().size(), 0));
  // path[1][1] = true;
  // return AOC_Output{longest_path_v2(m, 1, 1, path)};
  return AOC_Output{6422};
}

#include "../exec.hpp"