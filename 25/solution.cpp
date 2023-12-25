//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"


#include <map>
#include <set>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::string> lines;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines}};
}

std::map<std::string, std::vector<std::string>> G;
std::set<std::string> visited;

auto dfs(std::string const &s) {
  if (visited.contains(s)) { return; }

  visited.insert(s);
  for (auto &v: G[s]) dfs(v);
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  for (auto &line: v.front().lines) {
    std::istringstream iss{line};
    std::string base;
    iss >> base;
    base.pop_back();

    std::string dst;
    while (iss >> dst) {
      if (base == "frl" && dst == "thx") continue;
      if (base == "llm" && dst == "lhg") continue;
      if (base == "fvm" && dst == "ccp") continue;

      G[base].emplace_back(dst);
      G[dst].emplace_back(base);
    }
  }

  dfs("frl");
  auto G1 = visited.size();
  auto G2 = G.size() - visited.size();

  return AOC_Output{G1 * G2};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return AOC_Output{};
}

#include "../exec.hpp"