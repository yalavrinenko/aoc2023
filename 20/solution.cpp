//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <list>
#include <queue>

struct AOC_Output{
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<long> values;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(const std::vector<std::string> &lines) {
  AOC_Input in{};
  in.values = lines | vw::transform([](auto const &s) { return std::stol(s); }) | as_vector ;
  return {in};
}

auto mix(std::list<long> &values, std::vector<std::list<long>::iterator> &order){
  long N = values.size();
  for (auto cv: order){
    auto steps = *cv - (*cv / (N - 1)) * (N - 1);
    auto shift = (steps < 0) ? -1 : 1;
    auto it = cv;
    while (std::abs(steps) > 0){
      if (it == values.begin() && shift == -1)
        it = std::prev(values.end());
      else
        std::advance(it, shift);

      if (it == values.end())
        it = values.begin();
      if (it != cv)
        steps -= shift;
    }

    if (shift == 1){
      std::advance(it, shift);

      if (it == values.end())
        it = values.begin();
    }

    values.splice(it, values, cv);

//    fmt::print("{}= {}\n", *cv, fmt::join(values, " "));
  }
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  std::list<long> values;
  std::vector<std::list<long>::iterator> order;

  for (auto &e : v.front().values){
    values.emplace_back(e);
    order.emplace_back(std::prev(values.end()));
  }

  mix(values, order);

  auto zero = std::ranges::find(values, 0);
  auto zero_pos = std::ranges::distance(values.begin(), zero);

  auto value_at = [&zero_pos, &values](auto shift){
    auto pos = (zero_pos + shift) % values.size();
    auto it = values.begin();
    std::advance(it, pos);
    return *it;
  };

  fmt::print("{} {} {}\n", value_at(1000), value_at(2000), value_at(3000));

  return AOC_Output{value_at(1000) + value_at(2000) + value_at(3000)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  std::list<long> values;
  std::vector<std::list<long>::iterator> order;

  for (auto &e : v.front().values){
    values.emplace_back(e * 811589153);
    order.emplace_back(std::prev(values.end()));
  }

  for (auto i = 0; i < 10; ++i)
    mix(values, order);

  auto zero = std::ranges::find(values, 0);
  auto zero_pos = std::ranges::distance(values.begin(), zero);

  auto value_at = [&zero_pos, &values](auto shift){
    auto pos = (zero_pos + shift) % values.size();
    auto it = values.begin();
    std::advance(it, pos);
    return *it;
  };

  fmt::print("{} {} {}\n", value_at(1000), value_at(2000), value_at(3000));

  return AOC_Output{value_at(1000) + value_at(2000) + value_at(3000)};

  return AOC_Output{};
}

#include "../exec.hpp"