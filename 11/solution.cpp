//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <deque>
#include <functional>
#include <set>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::deque<uint64_t> items;
  std::function<uint64_t(uint64_t)> worry_update;
  std::function<uint64_t(uint64_t)> rethrow_function;
  long activities = 0;
  uint64_t div_v = 0;
};

auto reevaluate_function(char op, int value)
    -> std::function<uint64_t(uint64_t)> {
  switch (op) {
    case '+':
      return [value](uint64_t v) { return v + value; };
    case '-':
      return [value](uint64_t v) { return v - value; };
    case '*':
      return [value](uint64_t v) { return v * value; };
    case '/':
      return [value](uint64_t v) { return v / value; };
    default:
      throw "Logic error";
  };
}

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::lazy_split(std::string{}) |
         vw::transform([](const auto &monkey) {
           auto monkey_lines = monkey | vw::common;
           auto mit = monkey_lines.begin();
           std::advance(mit, 1);
           auto tmp_str = *mit;
           std::replace(tmp_str.begin(), tmp_str.end(), ',', ' ');
           std::istringstream iss{tmp_str};
           std::string trash;
           iss >> trash >> trash;
           AOC_Input in;
           in.items = std::deque(std::istream_iterator<uint64_t>{iss},
                                 std::istream_iterator<uint64_t>{});

           std::advance(mit, 1);
           int64_t value = {-1};
           char op;
           std::sscanf(mit->c_str(), "  Operation: new = old %c %ld",
                       &op, &value);
           if (value == -1)
             in.worry_update = [](uint64_t v) { return v * v; };
           else
             in.worry_update = reevaluate_function(op, value);

           std::advance(mit, 1);
           std::sscanf(mit->c_str(), "  Test: divisible by %ld",
                       &value);

           int true_throw, false_throw;
           std::advance(mit, 1);
           std::sscanf(mit->c_str(),
                       "      If true: throw to monkey %d",
                       &true_throw);

           std::advance(mit, 1);
           std::sscanf(mit->c_str(),
                       "      If false: throw to monkey %d",
                       &false_throw);

           in.div_v = value;
           in.rethrow_function = [value, true_throw,
                                  false_throw](uint64_t v) {
             return (v % value == 0) ? true_throw : false_throw;
           };

           return in;
         }) |
         as_vector;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto monkeys = v;
  for (auto it = 0; it < 20; ++it) {
    for (auto &monkey: monkeys) {
      while (!monkey.items.empty()) {
        auto item = monkey.items.front();
        monkey.items.pop_front();
        item = monkey.worry_update(item);
        item /= 3;
        monkeys[monkey.rethrow_function(item)].items.emplace_back(
            item);
        ++monkey.activities;
      }
    }
  }
  std::ranges::sort(monkeys, std::greater{}, &AOC_Input::activities);
  return AOC_Output{monkeys[0].activities * monkeys[1].activities};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::set dump{1,    20,   1000, 2000, 3000, 4000,
                5000, 6000, 7000, 8000, 9000, 10000};

  auto Q = std::accumulate(
      v.begin(), v.end(), 1UL,
      [](auto prod, auto const &e) { return prod * e.div_v; });

  auto monkeys = v;
  for (auto it = 0; it < 10000; ++it) {
    for (auto &monkey: monkeys) {
      while (!monkey.items.empty()) {
        auto item = monkey.items.front();
        monkey.items.pop_front();
        item = monkey.worry_update(item) % Q;
        monkeys[monkey.rethrow_function(item)].items.emplace_back(
            item);
        ++monkey.activities;
      }
    }
  }
  std::ranges::sort(monkeys, std::greater{}, &AOC_Input::activities);
  return AOC_Output{monkeys[0].activities * monkeys[1].activities};
}

#include "../exec.hpp"