//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <set>
#include <unordered_set>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct point {
  long x, y;
};

struct AOC_Input {
  point sensor, beacon;
  size_t distance;
};

constexpr auto distance(point a, point b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &line) {
           AOC_Input in{};
           std::sscanf(line.c_str(),
                       "Sensor at x=%ld, y=%ld: closest beacon is at "
                       "x=%ld, y=%ld",
                       &in.sensor.x, &in.sensor.y, &in.beacon.x,
                       &in.beacon.y);
           in.distance = distance(in.sensor, in.beacon);
           return in;
         }) |
         as_vector;
}

struct region {
  long start, end;
  bool operator<(const region &rhs) const {
    return start < rhs.start;
  }
};

auto beacon_clear_range(const std::vector<AOC_Input> &v,
                        long target_Y) {
  std::multiset<region> beacon_free_region;
  std::ranges::for_each(v, [&beacon_free_region,
                            target_Y](const AOC_Input &e) {
    auto start_x = e.sensor.x;
    region r{start_x, start_x};
    while (distance(e.sensor, point{r.end, target_Y}) <= e.distance) {
      ++r.end;
    }
    --r.end;

    while (distance(e.sensor, point{r.start, target_Y}) <=
           e.distance) {
      --r.start;
    }
    ++r.start;
    if (r.start <= r.end) beacon_free_region.insert(r);
  });
  std::vector<region> output;
  auto current = *beacon_free_region.begin();
  for (auto r: beacon_free_region) {
    if (current.end < r.start - 1) {
      output.emplace_back(current);
      current = r;
    } else {
      current.end = std::max(current.end, r.end);
    }
  }
  output.emplace_back(current);
  return output;
}

auto bounded_beacon_clear_range(const std::vector<AOC_Input> &v,
                        long target_Y) {
  std::multiset<region> beacon_free_region;
  std::ranges::for_each(v, [&beacon_free_region,
                            target_Y](const AOC_Input &e) {
    auto start_x = e.sensor.x;
    if (distance(e.sensor, point{start_x, target_Y}) > e.distance)
      return ;
    region r;

    region rbound{start_x, 4000000};
    while (rbound.end - rbound.start > 0){
      auto value = (rbound.end + rbound.start) / 2;
      if (distance(e.sensor, point{value, target_Y}) < e.distance)
        rbound.start = value + 1;
      if (distance(e.sensor, point{value, target_Y}) > e.distance)
        rbound.end = value - 1;
      if (distance(e.sensor, point{value, target_Y}) == e.distance)
        rbound.start = rbound.end = value;
    }
    r.end = rbound.start;

    rbound = region{0, start_x};
    while (rbound.end - rbound.start > 0){
      auto value = (rbound.end + rbound.start) / 2;
      if (distance(e.sensor, point{value, target_Y}) < e.distance)
        rbound.end = value - 1;
      if (distance(e.sensor, point{value, target_Y}) > e.distance)
        rbound.start = value + 1;
      if (distance(e.sensor, point{value, target_Y}) == e.distance)
        rbound.start = rbound.end = value;
    }
    r.start = rbound.start;

    beacon_free_region.insert(r);
  });
  std::vector<region> output;
  auto current = *beacon_free_region.begin();
  for (auto r: beacon_free_region) {
    if (current.end < r.start - 1) {
      output.emplace_back(current);
      current = r;
    } else {
      current.end = std::max(current.end, r.end);
    }
  }
  output.emplace_back(current);
  return output;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  static constinit auto target_Y = 2000000;
  auto beacon_in_range =
      v | vw::filter([](const auto &e) {
        return e.beacon.y == target_Y;
      }) |
      vw::transform([](const auto &e) { return e.beacon.x; }) | vw::common;

  auto bf_ranges = beacon_clear_range(v, target_Y);

  auto banned_places_count = std::accumulate(bf_ranges.begin(), bf_ranges.end(), 0, [](auto sum, auto range){
    return sum + (range.end - range.start) + 1;
  });
  return AOC_Output{banned_places_count - std::unordered_set(beacon_in_range.begin(), beacon_in_range.end()).size()};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  for (std::weakly_incrementable auto y : vw::iota(0, 4000000)){
    auto bf_ranges = bounded_beacon_clear_range(v, y);
    if (bf_ranges.size() > 1){
      return AOC_Output{(bf_ranges.front().end + 1) * 4000000UL + y};
    }
  }
  throw std::logic_error("Smth. goes wrong");
}

#include "../exec.hpp"