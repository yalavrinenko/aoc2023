//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <deque>

struct AOC_Output{
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string instr{};
  int value{};
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &l){
           AOC_Input ain;
           std::stringstream iss{l};
           iss >> ain.instr;
           if (ain.instr != "noop")
            iss >> ain.value;
           return ain;
         }) | as_vector;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  unsigned tick = 0;
  long X = 1;
  std::deque<unsigned> period{20, 60, 100, 140, 180, 220};
  long signal_strength_sum = 0;
  for (const auto &e : v){
    if (e.instr == "noop")
      tick += 1;
    if (e.instr == "addx"){
      tick += 2;
      X += e.value;
    }
    if (tick == period.front() || tick == period.front() + 1){
      auto XReg = X - (e.instr == "noop" ? 0 : e.value);
      signal_strength_sum += period.front() * XReg;
      period.pop_front();
    }
  }
  return AOC_Output{signal_strength_sum};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  unsigned tick = 0;
  long X = 1;
  auto states = v | vw::transform([&X, &tick](const auto &e){
                  if (e.instr == "noop")
                    tick += 1;
                  if (e.instr == "addx"){
                    tick += 2;
                    X += e.value;
                  }
                  return std::pair{tick, X};
                }) | vw::common;
  std::deque XReg(states.begin(), states.end());
  long last_plotted = 0;
  std::array<char, 240> display{};

  auto plot_display = [&display](){
    for (std::weakly_incrementable auto i : vw::iota(0, 6)) {
      for (std::weakly_incrementable auto j: vw::iota(0, 40))
        fmt::print("{}", display[i * 40 + j]);
      fmt::print("\n");
    }
  };

  for (auto pix_pos = 0; std::weakly_incrementable auto i : vw::iota(1, 241)){
    if (pix_pos % 40 - 1 <= last_plotted && last_plotted <= pix_pos % 40 + 1)
      display[pix_pos] = '#';
    else
      display[pix_pos] = '.';
    if (i == XReg.front().first){
      last_plotted = XReg.front().second;
      XReg.pop_front();
    }
    ++pix_pos;
  }

  plot_display();
  return AOC_Output{};
}

#include "../exec.hpp"