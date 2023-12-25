//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct Point {
  double x, y, z;
  double vx, vy, vz;
};

struct AOC_Input {
  Point p;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  auto ps =
      lines | vw::transform([](std::string line) {
        std::ranges::replace_if(
            line, [](char c) { return c == '@' || c == ','; }, ' ');
        std::istringstream iss{line};
        Point p{};
        iss >> p.x >> p.y >> p.z >> p.vx >> p.vy >> p.vz;
        return AOC_Input{p};
      }) |
      as_vector;
  return ps;
}

std::optional<Point> find_intersection(Point const &p1,
                                       Point const &p2) {
  double A = (p2.x - p1.x) / p1.vx;
  double B = p2.vx / p1.vx;

  double C = p1.y - p2.y + p1.vy * A;
  double D = p2.vy - p1.vy * B;

  auto tau = C / D;
  auto t = A + B * tau;

  Point res{};
  res.x = p1.x + p1.vx * t;
  res.y = p1.y + p1.vy * t;
  return (tau >= 0 && t >= 0) ? res : std::optional<Point>{};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  double RMin = 200000000000000;
  double RMax = 400000000000000;

  auto count = 0LL;

  for (auto i = 0; i < v.size() - 1; ++i)
    for (auto j = i + 1; j < v.size(); ++j) {
      auto inter = find_intersection(v[i].p, v[j].p);
      if (inter) {
        // fmt::print("{} vs. {}: [{}; {}]\n", i, j, inter->x, inter->y);
        if (RMin <= inter->x && inter->x <= RMax &&
            RMin <= inter->y && inter->y <= RMax) {
          ++count;
        }
      } else {
        // fmt::print("{} vs {} no intersect\n", i, j);
      }
    }
  return AOC_Output{count};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{};
}

#include "../exec.hpp"