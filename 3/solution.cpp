//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string first, second;
  explicit AOC_Input(const std::string &in)
      : first{in.substr(0, in.size() / 2)}, second{in.substr(
                                                in.size() / 2)} {
    std::ranges::sort(first);
    std::ranges::sort(second);
  }
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines |
         vw::transform([](const auto &s) { return AOC_Input(s); }) |
         as_vector;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto missed_item =
      v | vw::transform([](auto e) {
        std::vector<char> intersection;
        std::ranges::set_intersection(
            e.first, e.second, std::back_inserter(intersection));
        return (std::islower(intersection[0]))
                   ? intersection[0] - 'a' + 1
                   : intersection[0] - 'A' + 27;
      }) |
      as_vector;
  return AOC_Output{
      std::accumulate(missed_item.begin(), missed_item.end(), 0L)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::vector<char> badges;
  badges.reserve(v.size() / 3);
  for (std::weakly_incrementable auto i:
       vw::iota(0UL, v.size() / 3)) {
    auto group = std::ranges::subrange(&v[i * 3], &v[i * 3 + 3]) |
                 vw::transform([](const auto &e) {
                   auto backpack = e.first + e.second;
                   std::ranges::sort(backpack);
                   return backpack;
                 });
    auto intersection = (group | vw::take(1) | as_vector).front();
    std::ranges::for_each(group, [&intersection](const auto &e) {
      std::string s;
      std::ranges::set_intersection(e, intersection,
                                    std::back_inserter(s));
      intersection = std::move(s);
    });
    badges.emplace_back(intersection.front());
  }
  return AOC_Output{std::accumulate(
      badges.begin(), badges.end(), 0L, [](auto sum, auto e) {
        return sum + ((std::islower(e)) ? e - 'a' + 1 : e - 'A' + 27);
      })};
}

#include "../exec.hpp"