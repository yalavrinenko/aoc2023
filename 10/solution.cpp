//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <queue>
#include <set>

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
  return {AOC_Input{.map = lines}};
}

std::unordered_map<
    char, std::pair<std::pair<int, int>, std::pair<int, int>>>
    transition_map{
        {'|', {{-1, 0}, {1, 0}}}, {'-', {{0, -1}, {0, 1}}},
        {'L', {{-1, 0}, {0, 1}}}, {'J', {{0, -1}, {-1, 0}}},
        {'7', {{0, -1}, {1, 0}}}, {'F', {{0, 1}, {1, 0}}},
    };
std::unordered_map<char, std::pair<std::set<char>, std::set<char>>>
    transition_pipes{
        {'|', {{'|', 'F', '7'}, {'|', 'L', 'J'}}},
        {'-', {{'-', 'L', 'F'}, {'-', 'J', '7'}}},
        {'L', {{'|', 'F', '7'}, {'-', 'J', '7'}}},
        {'J', {{'-', 'L', 'F'}, {'|', 'F', '7'}}},
        {'7', {{'-', 'L', 'F'}, {'|', 'L', 'J'}}},
        {'F', {{'-', 'J', '7'}, {'|', 'L', 'J'}}},
    };

auto bfs(const std::vector<std::string> &cmap, std::pair<int, int> ij,
         char start_symbol) {
  auto map = cmap;
  struct vertex {
    std::pair<int, int> ij;
    uint64_t steps;
    char symbol;
  };
  std::queue<vertex> q;
  q.emplace(vertex{ij, 0, start_symbol});
  map[ij.first][ij.second] = '*';

  uint64_t max_step = 0ULL;

  while (!q.empty()) {
    auto v = q.front();
    q.pop();
    auto symbol = v.symbol;

    {
      const auto &step = transition_map[symbol].first;
      const auto &step_p = transition_pipes[symbol].first;

      auto i = v.ij.first + step.first;
      auto j = v.ij.second + step.second;
      if (i >= 0 && i < map.size() && j >= 0 &&
          j < map.front().size()) {
        if (step_p.contains(map[i][j])) {
          vertex vx{.ij = {i, j},
                    .steps = v.steps + 1,
                    .symbol = map[i][j]};
          q.emplace(vx);
          map[i][j] = '*';
        }
      }
    }

    {
      const auto &step = transition_map[symbol].second;
      const auto &step_p = transition_pipes[symbol].second;

      auto i = v.ij.first + step.first;
      auto j = v.ij.second + step.second;
      if (i >= 0 && i < map.size() && j >= 0 &&
          j < map.front().size()) {
        if (step_p.contains(map[i][j])) {
          vertex vx{.ij = {i, j},
                    .steps = v.steps + 1,
                    .symbol = map[i][j]};
          q.emplace(vx);
          map[i][j] = '*';
        }
      }
    }

    max_step = std::max(max_step, v.steps);
  }

  return std::pair{max_step, map};
}

std::pair<int, int> find_s(std::vector<std::string> const &map) {
  for (auto i = 0U; i < map.size(); ++i) {
    for (auto j = 0U; j < map[i].size(); ++j) {
      if (map[i][j] == 'S') { return {i, j}; }
    }
  }
  throw "Fuuuck!";
}

bool check_symbol(std::vector<std::string> const &map,
                  std::pair<int, int> ij, char sym) {
  auto [step_1, step_2] = transition_map[sym];

  bool flag = false;

  {
    auto [i, j] = ij;
    i += step_1.first;
    j += step_1.second;
    flag = transition_pipes[sym].first.contains(map[i][j]);
  }

  {
    auto [i, j] = ij;
    i += step_2.first;
    j += step_2.second;
    flag = flag & transition_pipes[sym].second.contains(map[i][j]);
  }

  return flag;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  static constexpr std::array pipes{'|', '-', 'L', 'J', '7', 'F'};

  uint64_t max_v = 0LL;
  auto ij = find_s(v.front().map);
  for (auto &p: pipes) {
    if (check_symbol(v.front().map, ij, p)) {
      auto [st, _] = bfs(v.front().map, ij, p);
      max_v = std::max(st, max_v);
      break;
    }
  }

  return AOC_Output{max_v};
}

auto cbfs(std::vector<std::string> &map, std::pair<int, int> s) {
  std::queue<std::pair<int, int>> q;
  q.emplace(s);
  map[s.first][s.second] = 'O';
  while (!q.empty()) {
    auto v = q.front();
    q.pop();
    for (auto kv: {std::pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}) {
      auto i = v.first + kv.first, j = v.second + kv.second;
      if (i >= 0 && i < map.size() && j >= 0 &&
          j < map.front().size() && map[i][j] != '*' &&
          map[i][j] != 'O') {
        map[i][j] = 'O';
        q.emplace(i, j);
      }
    }
  }
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  static constexpr std::array pipes{'|', '-', 'L', 'J', '7', 'F'};

  uint64_t max_v = 0LL;
  auto ij = find_s(v.front().map);
  for (auto &p: pipes) {
    if (check_symbol(v.front().map, ij, p)) {
      auto [st, map] = bfs(v.front().map, ij, p);
      max_v = std::max(st, max_v);

      for (auto i = 0; i < map.size(); ++i) {
        for (auto j = 0; j < map[i].size(); ++j) {
          if (map[i][j] == '*') break;
          if (map[i][j] != 'O') cbfs(map, std::pair{i, j});
        }

        for (long j = map[i].size() - 1; j >= 0; --j) {
          if (map[i][j] == '*') break;
          if (map[i][j] != 'O') cbfs(map, std::pair{i, j});
        }
      }

      for (auto j = 0; j < map.front().size(); ++j) {
        for (auto i = 0; i < map.size(); ++i) {
          if (map[i][j] == '*') break;
          if (map[i][j] != 'O') cbfs(map, std::pair{i, j});
        }

        for (long i = map.size() - 1; i >= 0; --i) {
          if (map[i][j] == '*') break;
          if (map[i][j] != 'O') cbfs(map, std::pair{i, j});
        }
      }

      for (auto &line: map) {
        for (auto &c: line) {
          if (c != '*' && c != 'O') { c = 'I'; }
        }
      }

      for (auto i = 0; i < map.size(); ++i) {
        for (auto j = 0; j < map[i].size(); ++j) {
          if (map[i][j] == '*') { map[i][j] = v.front().map[i][j]; }
        }
      }

      auto count = 0ULL;

      for (auto line: map | vw::drop(map.size() / 4) |
                          vw::take(map.size() / 2)) {
        for (auto sym: line | vw::drop(line.size() / 4) |
                           vw::take(line.size() / 2)) {
          count += (sym == 'I');
        }
      }

      return AOC_Output{count};
    }
  }
  return AOC_Output{0ULL};
}

#include "../exec.hpp"