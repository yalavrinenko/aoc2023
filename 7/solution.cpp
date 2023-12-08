//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <map>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string cards;
  uint32_t value;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::transform([](const auto &s) {
           std::istringstream iss{s};
           AOC_Input in;
           iss >> in.cards >> in.value;
           return in;
         }) |
         as_vector;
}

static std::map<std::array<int, 5>, int> combinations{
    {{0, 0, 0, 0, 5}, 10}, {{0, 0, 0, 1, 4}, 9}, {{0, 0, 0, 2, 3}, 8},
    {{0, 0, 1, 1, 3}, 7},  {{0, 0, 1, 2, 2}, 6}, {{0, 1, 1, 1, 2}, 5},
    {{1, 1, 1, 1, 1}, 4},
};

std::array<int, 5> combination(const std::string &s) {
  std::array<int, 5> cb{};
  std::unordered_map<char, int> m;
  for (auto c: s) { ++m[c]; }

  for (auto i = 0; auto &[k, v]: m) { cb[i++] = v; }

  std::ranges::sort(cb);
  return cb;
}

auto transform(char c) {
  if ('0' < c && c <= '9') return c - '0';
  switch (c) {
    case 'T':
      return 10;
    case 'J':
      return 11;
    case 'Q':
      return 12;
    case 'K':
      return 13;
    case 'A':
      return 14;
    default:
  }
  throw std::runtime_error{"You are a dog!"};
}

auto cmp = [](const AOC_Input &lhs, const AOC_Input &rhs) {
  auto lhs_score = combinations.at(combination(lhs.cards));
  auto rhs_score = combinations.at(combination(rhs.cards));

  if (lhs_score == rhs_score) {
    for (auto i = 0; i < 5; ++i) {
      if (transform(lhs.cards[i]) == transform(rhs.cards[i]))
        continue;

      return transform(lhs.cards[i]) < transform(rhs.cards[i]);
    }
  }

  return lhs_score < rhs_score;
};

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto in = v;

  std::ranges::sort(in, cmp);
  auto sum = 0LL;
  for (auto i = 1LL; auto &e: in) {
    sum += e.value * i;
    ++i;
  }
  return AOC_Output{sum};
}

std::array<int, 5> combination_joker(const std::string &s) {
  std::array<int, 5> cb{};
  std::unordered_map<char, int> m;
  for (auto c: s) { ++m[c]; }

  auto joker = 0;
  if (m.contains('J')) {
    joker = m['J'];
    m['J'] = 0;
  }

  auto maxe = std::ranges::max_element(
      m, std::less<int>{}, &std::pair<const char, int>::second);

  maxe->second += joker;

  for (auto i = 0; auto &[k, v]: m) { cb[i++] = v; }

  std::ranges::sort(cb);
  return cb;
}

auto transform_joker(char c) {
  if ('0' < c && c <= '9') return c - '0';
  switch (c) {
    case 'T':
      return 10;
    case 'J':
      return 1;
    case 'Q':
      return 12;
    case 'K':
      return 13;
    case 'A':
      return 14;
    default:
  }
  throw std::runtime_error{"You are a dog!"};
}

auto cmp_joker = [](const AOC_Input &lhs, const AOC_Input &rhs) {
  auto lhs_score = combinations.at(combination_joker(lhs.cards));
  auto rhs_score = combinations.at(combination_joker(rhs.cards));

  if (lhs_score == rhs_score) {
    for (auto i = 0; i < 5; ++i) {
      if (transform_joker(lhs.cards[i]) ==
          transform_joker(rhs.cards[i]))
        continue;

      return transform_joker(lhs.cards[i]) <
             transform_joker(rhs.cards[i]);
    }
  }

  return lhs_score < rhs_score;
};

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto in = v;

  std::ranges::sort(in, cmp_joker);
  auto sum = 0LL;
  for (auto i = 1LL; auto &e: in) {
    sum += e.value * i;
    ++i;
  }
  return AOC_Output{sum};
}

#include "../exec.hpp"