//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <numeric>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct map_entry {
  std::string entry;
  std::string left, right;
};

struct AOC_Input {
  std::string pattern;
  std::vector<map_entry> map;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  AOC_Input in{
      .pattern = lines.front(),
      .map = lines | vw::drop(2) |
             vw::transform([](std::string line) {
               map_entry entry;
               std::ranges::replace(line, '=', ' ');
               std::ranges::replace(line, ',', ' ');
               std::ranges::replace(line, '(', ' ');
               std::ranges::replace(line, ')', ' ');
               std::istringstream iss{line};
               iss >> entry.entry;
               iss >> entry.left >> entry.right;
               return entry;
             }) |
             as_vector,
  };
  return {in};
}

std::unordered_map<std::string, std::pair<std::string, std::string>>
construct_map(AOC_Input const &in) {
  std::unordered_map<std::string, std::pair<std::string, std::string>>
      map;
  for (auto &e: in.map) {
    map[e.entry] = std::make_pair(e.left, e.right);
  }
  return map;
}

std::string make_step(const auto &map, const std::string &from,
                      char dir) {
  return (dir == 'L') ? map.at(from).first : map.at(from).second;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  const auto &in = v.front();

  auto map = construct_map(in);

  auto steps = 0ULL;
  std::string entry = "AAA";
  while ((entry = make_step(map, entry,
                            in.pattern[steps % in.pattern.size()])) !=
         "ZZZ") {
    ++steps;
  }

  return AOC_Output{steps + 1};
}


AOC_Output part_2(std::vector<AOC_Input> const &v) {
  const auto &in = v.front();

  auto map = construct_map(in);

  auto fall2z_in = [&map, &pattern = in.pattern](std::string entry) {
    auto steps = 0LL;
    while (entry.back() != 'Z') {
      entry = make_step(map, entry, pattern[steps % pattern.size()]);
      steps++;
    }
    return steps;
  };

  std::vector<uint64_t> entries;
  for (const auto &[k, _]: map) {
    if (k.back() == 'A') { entries.emplace_back(fall2z_in(k)); }
  }

  auto lcm = entries.front();
  for (auto &e: entries | vw::drop(1)) { lcm = std::lcm(lcm, e); }


  return AOC_Output{lcm};
}

#include "../exec.hpp"