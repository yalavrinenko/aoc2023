//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <future>
#include <map>
#include <unordered_map>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  int bid;
  // 0 - ore, 1 - clay, 2 - obs, 3 - geode
  std::array<int, 4> robots[4] = {{0, 0, 0, 0},
                                  {0, 0, 0, 0},
                                  {0, 0, 0, 0},
                                  {0, 0, 0, 0}};
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &s) {
           AOC_Input in{};

           std::sscanf(s.c_str(),
                       "Blueprint %d: Each ore robot costs %d ore. "
                       "Each clay robot costs %d ore. Each obsidian "
                       "robot costs %d ore and %d clay. Each geode "
                       "robot costs %d ore and %d obsidian.",
                       &in.bid, &in.robots[0][0], &in.robots[1][0],
                       &in.robots[2][0], &in.robots[2][1],
                       &in.robots[3][0], &in.robots[3][2]);

           return in;
         }) |
         as_vector;
}

struct state {
  std::array<int, 4> ores;
  std::array<int, 4> robots;
  int time;
};

auto operator+(std::array<int, 4> const &lhs,
               std::array<int, 4> const &rhs) {
  std::array<int, 4> c;
  for (auto i = 0; i < 4; ++i) c[i] = lhs[i] + rhs[i];
  return c;
}

auto operator-(std::array<int, 4> const &lhs,
               std::array<int, 4> const &rhs) {
  std::array<int, 4> c;
  for (auto i = 0; i < 4; ++i) c[i] = lhs[i] - rhs[i];
  return c;
}

constexpr auto a2i(std::array<int, 4> const &a) {
  return ((uint64_t) a[0] << 48) | ((uint64_t) a[1] << 32) |
         ((uint64_t) a[2] << 16) | ((uint64_t) a[3]);
}

using state_map_opt = std::unordered_map<
    int,
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, int>>>;

using ore_map = std::unordered_map<int, int>;

ore_map omap;

auto T_eur(int time, std::array<int, 4> ores,
           std::array<int, 4> robots, AOC_Input const &blueprint,
           state_map_opt &smap, std::array<int, 4> const &max_ore) {
  if (time == 0) return ores[3];

  if (!smap.contains(time) || !smap[time].contains(a2i(ores)) ||
      !smap[time][a2i(ores)].contains(a2i(robots))) {
    // no changes
    auto best = T_eur(time - 1, ores + robots, robots, blueprint,
                      smap, max_ore);

    for (auto rid = 3; auto &req: blueprint.robots | vw::reverse) {
      bool can_build = true;
      for (auto i = 0; i < 4; ++i) {
        if (ores[i] < req[i]) {
          can_build = false;
          break;
        }
      }

      if (rid != 3) {
        if (robots[rid] > max_ore[rid] + 1) can_build = false;
        if (robots[rid] * time + ores[rid] >= time * max_ore[rid])
          can_build = false;
      }

      if (can_build) {
        auto nore = ores - req + robots;
        ++robots[rid];
        auto score =
            T_eur(time - 1, nore, robots, blueprint, smap, max_ore);
        --robots[rid];

        if (score > best) {
          best = score;
          if (rid == 3) break;
        }
      }
      --rid;
    }

    smap[time][a2i(ores)][a2i(robots)] = best;
  }

  return smap[time][a2i(ores)][a2i(robots)];
}

auto blueprint_quality_eur(AOC_Input blue_print, int times = 24) {
  state_map_opt smap;

  std::array<int, 4> max_ore{0, 0, 0, 1000};
  for (auto r = 0; r < 3; ++r)
    max_ore[r] = std::ranges::max(
        blue_print.robots |
        vw::transform([&r](auto const &v) { return v[r]; }));

  return T_eur(times, {0, 0, 0, 0}, {1, 0, 0, 0}, blue_print, smap,
               max_ore);
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return AOC_Output{0};
  auto sum = 0;
  for (auto &bp: v) {
    auto quality = blueprint_quality_eur(bp);
    fmt::print("{} {}\n", bp.bid, quality);
    sum += bp.bid * quality;
  }
  return AOC_Output{sum};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto func = [&v](auto i) {
    if (i == 0 || i == 1) return 1;
    auto quality = blueprint_quality_eur(v[i], 32);
    fmt::print("{} {}\n", v[i].bid, quality);
    return quality;
  };

  auto f1 = std::async(std::launch::deferred, func, 0);
  auto f2 = std::async(std::launch::deferred, func, 1);
  auto f3 = std::async(std::launch::deferred, func, 3);

  return AOC_Output{f1.get() * f2.get() * f3.get()};
}

/*
 * 1 0
2 2
3 2
4 2
5 6
6 4
7 13
8 5
9 0
10 0
11 1
12 1
13 3
14 4
15 5
16 1
17 2
18 3
19 1
20 9
21 0
22 1
23 1
24 7
25 0
26 0
27 6
28 7
29 1
30 12

Working time 86635963 μs

1 9
2 27
 * */


#include "../exec.hpp"