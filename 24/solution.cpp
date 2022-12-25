//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <queue>
#include <set>
#include <unordered_map>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct vec2 {
  long x, y;
  auto operator+(auto const &rhs) const {
    return vec2{x + rhs.x, y + rhs.y};
  }

  auto operator==(auto const &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  auto operator<(auto const &rhs) const {
    return (x == rhs.x) ? y < rhs.y : x < rhs.x;
  }
};

struct blizzard {
  vec2 p;
  vec2 d;
  unsigned long xsize, ysize;

  auto next() {
    p = p + d;
    if (p.x == xsize - 1) p.x = 1;
    if (p.x == 0) p.x = xsize - 2;

    if (p.y == ysize - 1) p.y = 1;
    if (p.y == 0) p.y = ysize - 2;
  }
};

inline std::unordered_map dir_map{
    std::pair{'>', vec2{0, 1}},
    std::pair{'<', vec2{0, -1}},
    std::pair{'^', vec2{-1, 0}},
    std::pair{'v', vec2{1, 0}},
};

struct AOC_Input {
  std::vector<blizzard> bz;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  AOC_Input in;
  for (auto i = 0; i < lines.size(); ++i)
    for (auto j = 0; j < lines[i].size(); ++j) {
      if (lines[i][j] != '.' && lines[i][j] != '#')
        in.bz.emplace_back(blizzard{vec2{i, j},
                                    dir_map.at(lines[i][j]),
                                    lines.size(), lines[i].size()});
    }
  return {in};
}

auto bfs(vec2 start, vec2 end, std::vector<blizzard> &bz) {
  struct state {
    vec2 position;
    long steps;
    long score;

    bool operator<(const state &rhs) const {
      return score > rhs.score;
    }
  };

  std::unordered_map<long, std::vector<blizzard>> blizzards_states;
  blizzards_states[0] = bz;

  auto score = [end](vec2 p) {
    return (std::abs(end.x - p.x) + std::abs(end.y - p.y));
  };

  std::priority_queue<state> Q;
  Q.emplace(state{start, 0, score(start)});

  struct vstate {
    vec2 p;
    long step;
    auto operator<(const vstate &rhs) const {
      if (!(p < rhs.p) && !(rhs.p < p)) return step < rhs.step;
      return p < rhs.p;
    }
  };

  std::set<vstate> visited;

  while (!Q.empty()) {
    auto current = Q.top();
    Q.pop();
    if (current.position == end) {
      bz = blizzards_states[current.steps];
      return current.steps;
    }

    if (!visited.contains(vstate{current.position, current.steps})) {
      visited.emplace(vstate{current.position, current.steps});
      if (!blizzards_states.contains(current.steps + 1)) {
        blizzards_states[current.steps + 1] =
            blizzards_states[current.steps];
        for (auto &blz: blizzards_states[current.steps + 1])
          blz.next();
      }

      auto &blz = blizzards_states[current.steps + 1];

      auto check_and_insert = [start, end, &Q, &score,
                               &blz](state &current, auto p) {
        auto N = blz.front().xsize, M = blz.front().ysize;
        if (p == start || p == end) {
          Q.emplace(state{p, current.steps + 1,
                          current.steps + 1 + score(p)});
        }
        if (p.x <= 0 || p.x >= N - 1 || p.y <= 0 || p.y >= M - 1)
          return;

        if (std::ranges::find(blz, p, &blizzard::p) != blz.end())
          return;

        Q.emplace(state{p, current.steps + 1,
                        current.steps + 1 + score(p)});
      };

      check_and_insert(current, current.position + vec2{0, 0});
      for (auto dv: {-1, 1}) {
        check_and_insert(current, current.position + vec2{0, dv});
        check_and_insert(current, current.position + vec2{dv, 0});
      }
    }
  }

  return 0L;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  long N = v.front().bz.front().xsize, M = v.front().bz.front().ysize;
  auto bz = v.front().bz;
  auto res = bfs(vec2{0, 1}, vec2{N - 1, M - 2}, bz);
  return AOC_Output{res};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  long N = v.front().bz.front().xsize, M = v.front().bz.front().ysize;
  auto bz = v.front().bz;
  auto start = vec2{0, 1};
  auto end = vec2{N - 1, M - 2};
  auto leg_1 = bfs(start, end, bz);
  auto leg_2 = bfs(end, start, bz);
  auto leg_3 = bfs(start, end, bz);
  return AOC_Output{leg_1 + leg_2 + leg_3};
}

#include "../exec.hpp"