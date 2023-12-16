//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <list>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string line;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  auto cmds =
      lines.front() | vw::split(',') | vw::transform([](auto r) {
        return std::string(r.begin(), r.end());
      }) |
      vw::transform([](auto s) { return AOC_Input{std::move(s)}; }) |
      as_vector;
  return cmds;
}

auto hash(std::string const &l) {
  uint64_t H = 0;
  for (auto c: l) {
    H += c;
    H *= 17;
    H = H % 256;
  }
  return H;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto r =
      v | vw::transform([](const auto &e) { return hash(e.line); });
  return AOC_Output{std::accumulate(r.begin(), r.end(), 0LL)};
}

struct lens {
  std::string label;
  int focal;
};

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::array<std::list<lens>, 256> boxes;

  for (auto cmd: v) {
    if (cmd.line.contains('-')) {
      auto lens_lable = cmd.line;
      lens_lable.pop_back();

      auto &box = boxes[hash(lens_lable)];
      auto it = std::ranges::find(box, lens_lable, &lens::label);
      if (it != box.end()) { box.erase(it); }
    }
    if (cmd.line.contains('=')) {
      std::ranges::replace(cmd.line, '=', ' ');
      std::istringstream iss{cmd.line};
      lens l;
      iss >> l.label >> l.focal;
      auto &box = boxes[hash(l.label)];
      auto it = std::ranges::find(box, l.label, &lens::label);
      if (it == box.end()) {
        box.push_back(l);
      } else {
        it->focal = l.focal;
      }
    }
  }

  auto power = 0LL;
  for (auto i = 0; i < boxes.size(); ++i) {
    auto slot = 1;
    for (auto &l: boxes[i]) {
      power += (i + 1) * slot * l.focal;
      slot++;
    }
  }

  return AOC_Output{power};
}

#include "../exec.hpp"