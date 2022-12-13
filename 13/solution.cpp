//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <any>

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

using message = std::vector<std::any>;

struct AOC_Input {
  message first, second;
};

auto parse_message(std::istringstream &iss) -> message {
  message m;
  char c;
  while (iss >> c && c != ']') {
    if (c == '[') {
      m.emplace_back(parse_message(iss));
    } else {
      long value = c - '0';
      while ('0' <= iss.peek() && iss.peek() <= '9') {
        iss >> c;
        value = value * 10 + (c - '0');
      }
      m.emplace_back(value);
    }
  }
  return m;
}

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines | vw::lazy_split(std::string{}) |
         vw::transform([](const auto &mstr) {
           auto cv = mstr | vw::common;
           auto first = *(cv.begin());
           auto second = *std::next(cv.begin());

           auto get_message = [](auto &str) {
             std::ranges::replace(str, ',', ' ');
             std::istringstream iss{str};
             return parse_message(iss);
           };

           return AOC_Input{get_message(first), get_message(second)};
         }) |
         as_vector;
}

enum class Order { Correct, Wrong, Unknown };

Order order_check(const message &lhs, const message &rhs) {
  long i = 0;

  auto to_message = [](auto &msg) -> decltype(auto) {
    if (msg.type() == typeid(long)) return message{msg};
    return std::any_cast<message>(msg);
  };

  while (i < lhs.size() && i < rhs.size()) {
    if (lhs[i].type() == typeid(long) &&
        rhs[i].type() == typeid(long)) {
      if (std::any_cast<long>(lhs[i]) < std::any_cast<long>(rhs[i]))
        return Order::Correct;
      if (std::any_cast<long>(lhs[i]) > std::any_cast<long>(rhs[i]))
        return Order::Wrong;
    } else {
      auto res = order_check(to_message(lhs[i]), to_message(rhs[i]));
      if (res != Order::Unknown) return res;
    }
    ++i;
  }

  if (lhs.size() < rhs.size()) return Order::Correct;
  if (lhs.size() > rhs.size()) return Order::Wrong;

  return Order::Unknown;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto index = 0;
  return AOC_Output{std::accumulate(
      v.begin(), v.end(), 0, [&index](auto sum, const auto &e) {
        ++index;
        if (order_check(e.first, e.second) == Order::Correct) {
          return sum + index;
        }
        return sum;
      })};
}

void print(const message &msg) {
  fmt::print("[");
  bool is_first = true;
  for (auto i = 0; i < msg.size(); ++i) {
    if (msg[i].type() == typeid(long))
      if (is_first) fmt::print("{}", std::any_cast<long>(msg[i]));
      else
        fmt::print(" {}", std::any_cast<long>(msg[i]));
    else
      print(std::any_cast<message>(msg[i]));
    is_first = false;
  }
  fmt::print("]");
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::vector<message> signal;
  std::ranges::for_each(v, [&signal](const auto &e) {
    signal.emplace_back(e.first);
    signal.emplace_back(e.second);
  });
  std::ranges::sort(signal, [](const auto &lhs, const auto &rhs) {
    return order_check(lhs, rhs) == Order::Correct;
  });

  auto make_message = [](auto str) {
    std::istringstream iss{std::move(str)};
    return parse_message(iss);
  };

  auto msg_1 = make_message("[[2]]");
  auto msg_2 = make_message("[[6]]");

  auto insert_position = [&signal](const auto &msg) {
    return std::distance(
        signal.begin(),
        std::ranges::find_if(signal, [&msg](auto const &e) {
          return order_check(e, msg) == Order::Wrong;
        }));
  };

  auto ipos_1 = insert_position(msg_1) + 1;
  auto ipos_2 = insert_position(msg_2) + 2;

  return AOC_Output{ipos_1 * ipos_2};
}

#include "../exec.hpp"