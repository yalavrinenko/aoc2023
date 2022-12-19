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

struct AOC_Input {
  std::string node;
  std::vector<std::string> tunnels;
  long flow;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const std::string &lines) {
           AOC_Input in;
           char buf[4];
           std::sscanf(
               lines.c_str(),
               "Valve %s has flow rate=%zu; tunnels lead to valves",
               buf, &in.flow);
           in.node = std::string(buf);
           in.tunnels = lines.substr(lines.find("to valve") + 9,
                                     lines.size()) |
                        vw::lazy_split(std::string(", ")) |
                        vw::transform([](auto str) {
                          auto cv = str | vw::drop_while([](auto c) {
                                      return c == ' ';
                                    }) |
                                    vw::common;
                          return std::string(cv.begin(), cv.end());
                        }) |
                        as_vector;
           return in;
         }) |
         as_vector;
}

auto distance(
    const std::unordered_map<
        std::string, std::pair<std::vector<std::string>, long>> &m,
    std::string const &src, std::string const &dst) {
  static std::unordered_map<std::string, long> cache;
  if (!cache.contains(src + " => " + dst)) {

    struct node {
      std::string name;
      long steps;
    };
    std::set<std::string> visited;
    std::queue<node> q;
    q.emplace(node{src, 0L});
    visited.insert(src);

    while (!q.empty() && q.front().name != dst) {
      auto current = q.front();
      q.pop();
      for (const auto &next: m.at(current.name).first) {
        if (!visited.contains(next)) {
          q.emplace(node{next, current.steps + 1});
          visited.insert(next);
        }
      }
    }

    cache[src + " => " + dst] = q.front().steps;
  }
  return cache[src + " => " + dst];
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  std::unordered_map<std::string,
                     std::pair<std::vector<std::string>, long>>
      map;
  std::vector<std::string> i2n;
  for (auto &e: v) {
    map[e.node] = std::make_pair(e.tunnels, e.flow);
    i2n.emplace_back(e.node);
  }


  std::vector<std::vector<long>> T(31,
                                   std::vector<long>(i2n.size(), 0));
  std::vector<int> is_open(i2n.size(), 0);
  fmt::print("{}\t{:10}\n", 0, fmt::join(i2n, "\t"));
  for (auto i = 2; i <= 30; ++i) {
    for (auto j = 0; j < i2n.size(); ++j) {
      auto travel_time = [&map, &i2n](auto from, auto to) {
        auto d1 = distance(map, i2n[from], i2n[to]);
        auto d2 = distance(map, "AA", i2n[from]);
        return d1 + d2;
      };

      auto candidates = vw::iota(0UL, i2n.size()) |
                        vw::filter([travel_time, j, i](auto dst) {
                          return travel_time(dst, j) <= i;
                        });

      T[i][j] = 0;

      for (auto src: candidates) {
        T[i][j] = std::max(T[i][j], T[i - travel_time(src, j)][src]);
      }

      if (!is_open[j]) {
        for (auto src: candidates) {
          auto d = travel_time(src, j);
          if (d > i - 1) continue;
          auto profit =
              T[i - d - 1][src] + (30 - i) * map[i2n[j]].second;
          if (T[i][j] < profit) {
            T[i][j] = profit;
            is_open[j] = true;
          }
        }
      }
    }
    fmt::print("{}\t{:10}\n", i, fmt::join(T[i], "\t"));
  }

  return AOC_Output{};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{};
}

#include "../exec.hpp"