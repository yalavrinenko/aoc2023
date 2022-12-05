//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <list>
#include <utility>

struct AOC_Output {
  std::string value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{std::move(v)} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::list<char>> stacks;

  struct ops {
    int count, from, to;
  };
  std::vector<ops> operations;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  std::vector<std::string> text_stacks =
      lines |
      vw::take_while([](const auto &e) { return !e.empty(); }) |
      as_vector;
  text_stacks.pop_back();
  AOC_Input ain;
  ain.stacks.resize(10);
  for (const auto &level: text_stacks) {
    for (auto i = 1, stack_id = 1; i < level.size();
         i += 4, ++stack_id)
      if (level[i] != ' ')
        ain.stacks[stack_id].emplace_back(level[i]);
  }

  std::ranges::for_each(ain.stacks, [](auto &s) { s.reverse(); });

  ain.operations = lines | vw::drop(text_stacks.size() + 2) |
                   vw::transform([](const auto &e) {
                     AOC_Input::ops op{};
                     std::sscanf(e.c_str(), "move %d from %d to %d",
                                 &op.count, &op.from, &op.to);
                     return op;
                   }) |
                   as_vector;

  return {ain};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto corgo = v.front();
  std::ranges::for_each(
      corgo.operations, [&stacks = corgo.stacks](const auto &op) {
        auto first_it = stacks[op.from].end();
        std::advance(first_it, -op.count);
        std::list<char> tmp;
        tmp.splice(tmp.end(), stacks[op.from], first_it,
                   stacks[op.from].end());
        tmp.reverse();
        stacks[op.to].splice(stacks[op.to].end(), tmp);
      });
  return AOC_Output{
      std::accumulate(corgo.stacks.begin(), corgo.stacks.end(),
                      std::string{}, [](auto sum, const auto &l) {
                        return l.empty() ? sum : sum + l.back();
                      })};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto corgo = v.front();
  std::ranges::for_each(
      corgo.operations, [&stacks = corgo.stacks](const auto &op) {
        auto first_it = stacks[op.from].end();
        std::advance(first_it, -op.count);
        stacks[op.to].splice(stacks[op.to].end(), stacks[op.from],
                             first_it, stacks[op.from].end());
      });
  return AOC_Output{
      std::accumulate(corgo.stacks.begin(), corgo.stacks.end(),
                      std::string{}, [](auto sum, const auto &l) {
                        return l.empty() ? sum : sum + l.back();
                      })};
}

#include "../exec.hpp"