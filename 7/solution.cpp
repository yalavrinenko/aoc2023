//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <fmt/format.h>
#include <functional>
#include <stack>

struct AOC_Output {
  unsigned long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string command;
  std::vector<std::string> output;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  std::vector<AOC_Input> ain;
  for (auto i = 0U; i < lines.size(); ++i) {
    AOC_Input e{lines[i], {}};
    while (i < lines.size() - 1 && lines[i + 1][0] != '$') {
      e.output.emplace_back(lines[i + 1]);
      ++i;
    }
    ain.emplace_back(e);
  }
  return {ain};
}

class FS {
public:
  FS() = default;

  void run_command(const AOC_Input &ain) {
    std::istringstream iss{ain.command};
    iss.ignore(1);
    std::string cmd, arg;
    iss >> cmd >> arg;
    fs_commands_.at(cmd)(std::move(arg), ain);
  }

  void print(const std::string &root, unsigned ident = 0) const {
    if (ident > 200) throw "broken";

    std::string ident_str(ident, '-');
    for (const auto &[name, dir]: fs_.at(root)->subdirs) {
      fmt::print("{} {}/\n", ident_str, name);
      print(name, ident + 5);
    }

    for (const auto &[name, size]: fs_.at(root)->files)
      fmt::print("{} {} {}\n", ident_str, size, name);
  }

  size_t directory_size(const std::string &dname) {
    if (!dir_size_.contains(dname)) {
      auto &dir = fs_[dname];
      dir_size_[dname] =
          std::accumulate(dir->subdirs.begin(), dir->subdirs.end(),
                          0UL, [this](auto sum, const auto &kv) {
                            return sum + directory_size(kv.first);
                          });

      dir_size_[dname] += std::accumulate(
          dir->files.begin(), dir->files.end(), 0UL,
          [](auto sum, const auto &kv) { return sum + kv.second; });
    }

    return dir_size_[dname];
  }

  decltype(auto) directory_list() const {
    return fs_ |
           vw::transform([](const auto &kv) { return kv.first; });
  }

private:
  std::unordered_map<
      std::string_view,
      std::function<void(std::string, const AOC_Input &)>>
      fs_commands_ = {
          std::pair{"cd",
                    [this](auto arg, const auto &ain) {
                      if (arg == "..") current_.pop_back();
                      else if (arg == "/") {
                        current_ = std::deque<std::string>{{"/"}};
                      } else
                        current_.push_back(std::move(arg));
                    }},
          std::pair{"ls",
                    [this](const auto &arg, const auto &ain) {
                      auto &cdir = fs_[abs_path()];
                      for (const auto &e: ain.output) {
                        std::istringstream iss{e};
                        std::string name;
                        if (e[0] == 'd') {
                          iss >> name >> name;
                          add_directory(cdir, abs_path(name));
                        } else {
                          size_t size;
                          iss >> size >> name;
                          cdir->files.emplace_back(std::move(name),
                                                   size);
                        }
                      }
                    }},
  };

  void add_directory(auto &parent, auto dir_name) {
    if (!fs_.contains(dir_name)) {
      fs_.emplace(dir_name, std::make_shared<Directory>());
    }
    parent->subdirs[dir_name] = fs_[dir_name];
  }

  [[nodiscard]] std::string
  abs_path(const std::string &path = "") const {
    return (path.empty())
               ? fmt::format("{}", fmt::join(current_, "/"))
               : fmt::format("{}/{}", fmt::join(current_, "/"), path);
  }

  struct Directory {
    std::unordered_map<std::string, std::shared_ptr<Directory>>
        subdirs;
    std::vector<std::pair<std::string, unsigned>> files;
  };

  std::unordered_map<std::string, std::shared_ptr<Directory>> fs_{
      {"/", std::make_shared<Directory>()}};
  std::unordered_map<std::string, uint64_t> dir_size_{};

  std::deque<std::string> current_{{"/"}};
};

inline auto build_fs(const auto &in) {
  FS fs;
  for (const auto &e: in) fs.run_command(e);
  //fs.print("/");

  return fs;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto fs = build_fs(v);

  auto sum = 0UL;
  for (auto value:
       fs.directory_list() | vw::transform([&fs](const auto &e) {
         return fs.directory_size(e);
       }) | vw::filter([](auto v) { return v <= 100000; })) {
    sum += value;
  }

  return AOC_Output{sum};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto fs = build_fs(v);
  static constexpr auto FS_SIZE = 70000000, FS_FREE_REQ = 30000000;
  auto need_free = FS_FREE_REQ - (FS_SIZE - fs.directory_size("/"));
  auto candidates =
      fs.directory_list() | vw::transform([&fs](const auto &e) {
        return fs.directory_size(e);
      }) |
      vw::filter([need_free](auto v) { return v >= need_free; });
  return AOC_Output(std::ranges::min(candidates));
}

#include "../exec.hpp"
