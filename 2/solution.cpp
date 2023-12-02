//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  uint32_t game;
  std::string line;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const std::string &e) {
           uint32_t game;
           std::sscanf(e.c_str(), "Game %u", &game);
           return AOC_Input{.game = game,
                            .line = e.substr(e.find(':') + 1)};
         }) |
         as_vector;
}

struct Round {
  uint32_t r, g, b;
};

auto parse_game(const std::string &record) {
  auto rounds =
      record | vw::lazy_split(';') | vw::transform([](auto round) {
        auto balls = round | vw::lazy_split(',');

        Round r{};

        for (auto ball: balls) {
          auto cr = ball | vw::common;
          std::istringstream iss{std::string(cr.begin(), cr.end())};
          uint32_t count;
          std::string color;
          iss >> count >> color;
          if (color == "red") {
            r.r = count;
          } else if (color == "blue") {
            r.b = count;
          } else if (color == "green") {
            r.g = count;
          }
        }
        return r;
      }) |
      as_vector;
  return rounds;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  static constexpr auto R = 12, G = 13, B = 14;
  auto games =
      v | vw::transform([](auto &e) {
        return std::pair{e.game, parse_game(e.line)};
      }) |
      vw::filter([](const auto &e) {
        return std::ranges::all_of(e.second, [](auto &round) {
          return round.r <= R && round.g <= G && round.b <= B;
        });
      }) |
      vw::transform([](const auto &e) { return e.first; });

  return AOC_Output{
      std::accumulate(games.begin(), games.end(), 0ULL)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto powers =
      v | vw::transform([](auto &e) { return parse_game(e.line); }) |
      vw::transform([](const auto &e) {
        uint32_t R = 0, G = 0, B = 0;
        for (auto round: e) {
          R = std::max(R, round.r);
          G = std::max(G, round.g);
          B = std::max(B, round.b);
        }
        return static_cast<uint64_t>(R) * G * B;
      });
  
  return AOC_Output{
      std::accumulate(powers.begin(), powers.end(), 0ULL)};
}

#include "../exec.hpp"