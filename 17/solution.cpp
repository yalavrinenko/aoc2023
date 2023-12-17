//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <map>
#include <queue>

struct AOC_Output {
  long value{};
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

auto find_path(Map const &m) {
  struct State {
    int x, y;
    std::pair<int, int> dir;
    int dir_steps;

    int score;

    bool operator<(State const &s) const { return score > s.score; }
  };

  std::vector<std::vector<std::array<std::array<uint64_t, 3>, 4>>> D(
      m.size(), std::vector<std::array<std::array<uint64_t, 3>, 4>>(
                    m.front().size()));

  for (auto &sv: D) {
    for (auto &arr: sv) {
      for (auto i = 0; i < 4; ++i)
        arr[i] = std::array<uint64_t, 3>{
            10'000'000'000, 10'000'000'000, 10'000'000'000};
    }
  }

  std::priority_queue<State> q;

  auto commit_state = [&q, &D, &m](State s) {
    static std::map<std::pair<int, int>, int> M{
        {{-1, 0}, 0},
        {{1, 0}, 1},
        {{0, -1}, 2},
        {{0, 1}, 3},
    };

    if (s.x < 0 || s.y < 0 || s.x >= D.front().size() ||
        s.y >= D.size())
      return;

    s.score += m[s.y][s.x] - '0';
    if (D[s.y][s.x][M.at(s.dir)][s.dir_steps - 1] > s.score) {
      D[s.y][s.x][M.at(s.dir)][s.dir_steps - 1] = s.score;
      q.emplace(s);
    }
  };

  commit_state(State{.x = 1,
                     .y = 0,
                     .dir = {1, 0},
                     .dir_steps = 1,
                     .score = 0});

  commit_state(State{.x = 0,
                     .y = 1,
                     .dir = {0, 1},
                     .dir_steps = 1,
                     .score = 0});

  while (!q.empty()) {
    auto v = q.top();
    q.pop();
    if (v.x == m.front().size() - 1 && v.y == m.size() - 1) {
      return v.score;
    }

    if (v.dir_steps != 3) {
      State next{.x = v.x + v.dir.first,
                 .y = v.y + v.dir.second,
                 .dir = v.dir,
                 .dir_steps = v.dir_steps + 1,
                 .score = v.score};
      commit_state(next);
    }

    {
      auto ndir = std::pair{-v.dir.second, -v.dir.first};
      State next{.x = v.x + ndir.first,
                 .y = v.y + ndir.second,
                 .dir = ndir,
                 .dir_steps = 1,
                 .score = v.score};
      commit_state(next);
    }

    {
      auto ndir = std::pair{v.dir.second, v.dir.first};
      State next{.x = v.x + ndir.first,
                 .y = v.y + ndir.second,
                 .dir = ndir,
                 .dir_steps = 1,
                 .score = v.score};
      commit_state(next);
    }
  }

  throw "Fuck!";
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return AOC_Output{find_path(v.front().map)};
}

auto find_path_v2(Map const &m) {
  struct State {
    int x, y;
    std::pair<int, int> dir;
    int dir_steps;

    int score;

    bool operator<(State const &s) const { return score > s.score; }
  };

  std::vector<std::vector<std::array<std::array<uint64_t, 10>, 4>>> D(
      m.size(), std::vector<std::array<std::array<uint64_t, 10>, 4>>(
                    m.front().size()));

  for (auto &sv: D) {
    for (auto &arr: sv) {
      for (auto i = 0; i < 4; ++i)
        arr[i] = std::array<uint64_t, 10>{
            10'000'000'000, 10'000'000'000, 10'000'000'000,
            10'000'000'000, 10'000'000'000, 10'000'000'000,
            10'000'000'000, 10'000'000'000, 10'000'000'000,
            10'000'000'000,
        };
    }
  }

  std::priority_queue<State> q;

  auto commit_state = [&q, &D, &m](State s) {
    static std::map<std::pair<int, int>, int> M{
        {{-1, 0}, 0},
        {{1, 0}, 1},
        {{0, -1}, 2},
        {{0, 1}, 3},
    };

    if (s.x < 0 || s.y < 0 || s.x >= D.front().size() ||
        s.y >= D.size())
      return;

    s.score += m[s.y][s.x] - '0';
    if (D[s.y][s.x][M.at(s.dir)][s.dir_steps - 1] > s.score) {
      D[s.y][s.x][M.at(s.dir)][s.dir_steps - 1] = s.score;
      q.emplace(s);
    }
  };

  commit_state(State{.x = 1,
                     .y = 0,
                     .dir = {1, 0},
                     .dir_steps = 1,
                     .score = 0});

  commit_state(State{.x = 0,
                     .y = 1,
                     .dir = {0, 1},
                     .dir_steps = 1,
                     .score = 0});

  while (!q.empty()) {
    auto v = q.top();
    q.pop();
    if (v.x == m.front().size() - 1 && v.y == m.size() - 1) {
      return v.score;
    }

    if (v.dir_steps != 10) {
      State next{.x = v.x + v.dir.first,
                 .y = v.y + v.dir.second,
                 .dir = v.dir,
                 .dir_steps = v.dir_steps + 1,
                 .score = v.score};
      commit_state(next);
    }

    if (v.dir_steps >= 4) {
      {
        auto ndir = std::pair{-v.dir.second, -v.dir.first};
        State next{.x = v.x + ndir.first,
                   .y = v.y + ndir.second,
                   .dir = ndir,
                   .dir_steps = 1,
                   .score = v.score};
        commit_state(next);
      }

      {
        auto ndir = std::pair{v.dir.second, v.dir.first};
        State next{.x = v.x + ndir.first,
                   .y = v.y + ndir.second,
                   .dir = ndir,
                   .dir_steps = 1,
                   .score = v.score};
        commit_state(next);
      }
    }
  }

  throw "Fuck!";
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{find_path_v2(v.front().map)};
}

#include "../exec.hpp"