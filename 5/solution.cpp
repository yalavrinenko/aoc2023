//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <map>

struct AOC_Output {
  uint64_t value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct ranges {
  uint64_t from, to, size;
};

struct tfm_ctx {
  std::string from, to;
  std::vector<ranges> tfm;
};

struct AOC_Input {
  std::vector<uint64_t> seeds;
  std::vector<tfm_ctx> tfms;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  auto seeds_line_stream = std::istringstream(
      lines.front().substr(lines.front().find(':') + 1));
  AOC_Input in;
  in.seeds =
      std::vector(std::istream_iterator<uint64_t>{seeds_line_stream},
                  std::istream_iterator<uint64_t>{});

  tfm_ctx ctx;
  for (const auto &line: lines | vw::drop(2)) {
    if (line.contains("map")) {
      auto src = std::make_unique<char[]>(255);
      auto dst = std::make_unique<char[]>(255);
      auto cline = line;
      std::ranges::replace(cline, '-', ' ');
      std::sscanf(cline.c_str(), "%s to %s map:", src.get(),
                  dst.get());
      ctx.from = std::string(src.get());
      ctx.to = std::string(dst.get());
    } else if (line.empty()) {
      std::ranges::sort(ctx.tfm, std::less{}, &ranges::from);
      in.tfms.emplace_back(ctx);
      ctx.tfm.clear();
    } else {
      std::istringstream iss{line};
      ranges tfm_range{};
      iss >> tfm_range.to >> tfm_range.from >> tfm_range.size;
      ctx.tfm.emplace_back(tfm_range);
    };
  }
  return {in};
}

template<typename T>
void apply_transformation(
    std::vector<T> &seeds,
    std::map<std::string, tfm_ctx> const &seed_tfm,
    std::string const &entry) {
  if (!seed_tfm.contains(entry)) return;
  const auto &tfms = seed_tfm.at(entry).tfm;
  for (auto &v: seeds) {
    auto in_range = [v](const ranges &r) {
      if (r.from <= v && v < r.from + r.size) { return true; }
      return false;
    };
    auto it = std::ranges::find_if(tfms, in_range);
    if (it != tfms.end()) { v = it->to + (v - it->from); }
  }

  return apply_transformation(seeds, seed_tfm, seed_tfm.at(entry).to);
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  const auto &in = v.front();

  std::map<std::string, tfm_ctx> seed_tfm;
  for (auto &tfm: in.tfms) { seed_tfm[tfm.from] = tfm; }

  auto seeds = in.seeds;
  apply_transformation(seeds, seed_tfm, "seed");

  return AOC_Output{*std::ranges::min_element(seeds)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  const auto &in = v.front();

  std::map<std::string, tfm_ctx> seed_tfm;
  for (auto &tfm: in.tfms) { seed_tfm[tfm.from] = tfm; }

  std::vector<uint32_t> seeds;

  for (auto i = 0UL; i < in.seeds.size(); i += 2) {
    uint32_t value = in.seeds[i];
    std::generate_n(std::back_inserter(seeds), in.seeds[i + 1],
                    [&value]() { return value++; });
  }

  apply_transformation(seeds, seed_tfm, "seed");

  return AOC_Output{*std::ranges::min_element(seeds)};
}

#include "../exec.hpp"