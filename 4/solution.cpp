//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <set>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  uint32_t card;
  std::vector<uint32_t> winners;
  std::vector<uint32_t> values;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  auto in =
      lines | vw::transform([](std::string const &line) {
        uint32_t card;
        std::sscanf(line.c_str(), "Card %u", &card);
        auto wline = line.substr(line.find(':') + 1,
                                 line.find('|') - line.find(':') - 1);
        auto winners = std::istringstream(wline);
        auto vline = line.substr(line.find('|') + 1);
        auto values = std::istringstream(vline);

        AOC_Input in_cards{.card = card, .winners = {}, .values = {}};
        std::copy(std::istream_iterator<uint32_t>{winners},
                  std::istream_iterator<uint32_t>{},
                  std::back_inserter(in_cards.winners));
        std::copy(std::istream_iterator<uint32_t>{values},
                  std::istream_iterator<uint32_t>{},
                  std::back_inserter(in_cards.values));
        return in_cards;
      }) |
      as_vector;
  return in;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto scores =
      v | vw::transform([](AOC_Input in) {
        std::ranges::sort(in.winners);
        std::ranges::sort(in.values);

        std::vector<uint32_t> intersection;
        std::ranges::set_intersection(
            in.winners, in.values, std::back_inserter(intersection));

        return (intersection.empty())
                   ? 0ULL
                   : 1ULL << (intersection.size() - 1);
      });
  return AOC_Output{
      std::accumulate(scores.begin(), scores.end(), 0ULL)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto cards =
      v | vw::transform([](AOC_Input in) {
        std::ranges::sort(in.winners);
        std::ranges::sort(in.values);

        std::vector<uint32_t> intersection;
        std::ranges::set_intersection(
            in.winners, in.values, std::back_inserter(intersection));

        return std::pair{intersection.size(), 1ULL};
      }) |
      as_vector;

  for (auto i = 0; i < cards.size(); ++i) {
    for (auto j = i + 1; j < i + cards[i].first + 1; ++j) {
      cards[j].second += cards[i].second;
    }
  }

  auto cards_count =
      cards | vw::transform([](auto const &v) { return v.second; });

  return AOC_Output{
      std::accumulate(cards_count.begin(), cards_count.end(), 0ULL)};
}

#include "../exec.hpp"