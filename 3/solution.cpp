//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <regex>
#include <set>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<std::string> map;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return {AOC_Input{lines}};
}

struct number {
  uint64_t value;
  struct {
    long begin, end;
  } cols;
};

auto extract_numbers(const std::string &line) {
  std::regex r("\\d+");
  std::vector<number> numbers;
  std::ranges::transform(
      std::sregex_iterator{line.begin(), line.end(), r},
      std::sregex_iterator{}, std::back_inserter(numbers),
      [](decltype(*std::sregex_iterator()) match) {
        number n{};
        n.value = std::stoll(match.str());
        n.cols = {
            .begin = match.position(),
            .end = match.position() + match.length(),
        };
        return n;
      });
  return numbers;
}

auto extract_symbols(const std::string &line) {
  std::vector<uint32_t> symbols;
  for (auto i = 0U; i < line.size(); ++i) {
    if (!isdigit(line[i]) && line[i] != '.') {
      symbols.emplace_back(i);
    }
  }
  return symbols;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  std::vector<std::vector<number>> numbers;
  std::vector<std::vector<uint32_t>> symbols;

  for (auto &e: v.front().map) {
    numbers.emplace_back(extract_numbers(e));
    symbols.emplace_back(extract_symbols(e));
  }
  std::vector<uint64_t> engine_parts;

  long row = 0;
  for (const auto &nline: numbers) {
    auto eng_parts =
        nline |
        vw::filter([begin = std::max(row - 1, 0L),
                    end = std::min(row + 1, static_cast<long>(
                                                numbers.size() - 1)),
                    &symbols](const number &n) {
          for (auto i = begin; i <= end; ++i) {
            if (std::any_of(symbols[i].begin(), symbols[i].end(),
                            [&n](auto spos) {
                              return n.cols.begin - 1 <= spos &&
                                     spos <= n.cols.end;
                            })) {
              return true;
            }
          }
          return false;
        }) |
        vw::transform([](const auto &n) { return n.value; });
    engine_parts.insert(engine_parts.end(), eng_parts.begin(),
                        eng_parts.end());
    ++row;
  }

  return AOC_Output{
      std::accumulate(engine_parts.begin(), engine_parts.end(), 0L)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::vector<std::vector<number>> numbers;
  std::vector<std::vector<uint32_t>> stars;

  for (auto &e: v.front().map) {
    numbers.emplace_back(extract_numbers(e));
    stars.emplace_back();
    for (auto i = 0U; i < e.size(); ++i) {
      if (e[i] == '*') { stars.back().emplace_back(i); }
    }
  }

  std::vector<uint64_t> gear_ration;
  for (auto row = 0UL; row < stars.size(); ++row) {
    for (auto spos: stars[row]) {
      std::vector<uint64_t> parts;
      auto begin = std::max(static_cast<long>(row) - 1, 0L);
      auto end = std::min(row + 1, numbers.size() - 1);
      for (auto i = begin; i <= end; ++i) {
        for (auto &part: numbers[i]) {
          if (part.cols.begin - 1 <= spos && spos <= part.cols.end) {
            parts.emplace_back(part.value);
          }
        }
      }
      if (parts.size() == 2) {
        gear_ration.emplace_back(parts.front() * parts.back());
      }
    }
  }

  return AOC_Output{
      std::accumulate(gear_ration.begin(), gear_ration.end(), 0L)};
}

#include "../exec.hpp"