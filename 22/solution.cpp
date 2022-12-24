//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <map>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::string> map;
  std::string instr;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  AOC_Input in;
  in.map = lines |
           vw::take_while([](auto const &s) { return !s.empty(); }) |
           as_vector;

  in.instr = lines.back();
  return std::vector<AOC_Input>{std::move(in)};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto map = v.front().map;

  for (auto &kv: map) kv = " " + kv + " ";

  map.push_back(std::string(100, ' '));
  map.insert(map.begin(), std::string(100, ' '));

  auto pos = std::pair<int, int>{
      1, std::ranges::distance(map[1].begin(),
                               std::ranges::find(map[1], '.'))};
  auto direction = std::pair{0, 1};

  std::istringstream iss{v.front().instr + "!"};

  auto get_int = [&iss]() {
    int v = 0;
    while ('0' <= iss.peek() && iss.peek() <= '9') {
      char c;
      iss >> c;
      v = v * 10 + (c - '0');
    }
    return v;
  };

  auto rotate = [](auto orig, char d) {
    auto [x, y] = orig;

    if (d == 'R') {
      return std::pair{-y, x};
    } else {
      return std::pair{y, -x};
    }
  };

  auto wrap = [&map](auto pos, auto direction) {
    auto x = pos.first, y = pos.second;
    auto dx = direction.first, dy = direction.second;
    dx *= -1;
    dy *= -1;

    if (0 <= x && x < map.size()) {
      if ((y <= 0 || y >= map[x].size()) || map[x][y] == ' ') {
        x -= dx;
        y += dy;
        while ((0 <= y && y < map[x].size()) && map[x][y] != ' ') {
          x -= dx;
          y += dy;
        }

        x += dx;
        y -= dy;
      }
    }
    return std::pair{x, y};
  };

  while (iss.peek() != '!') {
    if (iss.peek() == 'L' || iss.peek() == 'R') {
      char c;
      iss >> c;
      direction = rotate(direction, c);
    } else {
      auto steps = get_int();
      while (steps > 0) {
        auto next = wrap(std::pair{pos.first - direction.first,
                                   pos.second + direction.second},
                         direction);
        if (map[next.first][next.second] == '.') pos = next;
        else
          steps = 1;
        --steps;
      }
    }
  }

  fmt::print("{} {} {} {}\n", pos.first, pos.second, direction.first,
             direction.second);

  std::map direction_score{std::pair{std::pair<int, int>{0, 1}, 0},
                           {{-1, 0}, 1},
                           {{0, -1}, 2},
                           {{1, 0}, 3}};

  return AOC_Output{1000 * pos.first + 4 * pos.second +
                    direction_score[direction]};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  fmt::print("Use python dude. Fuck this day\n");
  return AOC_Output{0};
}

#include "../exec.hpp"