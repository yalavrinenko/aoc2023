//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

struct Operation {
  std::string arg1, arg2;
  char op;
};

using expression =
    std::unordered_map<std::string, std::variant<double, Operation>>;

struct AOC_Input {
  expression expr;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {

  auto entry =
      lines | vw::transform([](const std::string &s) {
        std::istringstream iss{s};
        std::string var;
        iss >> var;
        var.pop_back();
        iss.ignore();
        if ('0' <= iss.peek() && iss.peek() <= '9') {
          double value;
          iss >> value;
          return std::pair{var,
                           std::variant<double, Operation>(value)};
        }

        Operation op;
        iss >> op.arg1 >> op.op >> op.arg2;
        return std::pair{var, std::variant<double, Operation>(op)};
      }) |
      vw::common;

  return {
      AOC_Input{std::unordered_map<std::string,
                                   std::variant<double, Operation>>(
          entry.begin(), entry.end())}};
}

double evaluate(expression &expr, std::string const &base) {
  auto visitor = [&expr](auto &&v) -> double {
    if constexpr (std::is_same_v<decltype(v), double &>) { return v; }

    if constexpr (std::is_same_v<decltype(v), Operation &>) {
      auto value_1 = evaluate(expr, v.arg1);
      auto value_2 = evaluate(expr, v.arg2);
      switch (v.op) {
        case '+':
          return value_1 + value_2;
        case '-':
          return value_1 - value_2;
        case '*':
          return value_1 * value_2;
        case '/':
          return value_1 / value_2;
        default:
          throw std::logic_error("Wrong op");
      }
    }

    throw std::logic_error("Wrong visitor type");
  };

  auto result = std::visit(visitor, expr[base]);

  expr[base] = result;

  return result;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto expr = v.front().expr;
  long result = evaluate(expr, "root");
  return AOC_Output{result};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto low = 1e+10;
  auto high = 1e+20;
  while (low <= high) {
    auto middle = std::floor((low + high) / 2);
    auto expr = v.front().expr;
    std::get<Operation>(expr["root"]).op = '-';
    std::get<double>(expr["humn"]) = middle;

    long result = evaluate(expr, "root");

    if (result == 0) { return AOC_Output{static_cast<long>(middle)}; }
    if (result < 0) high = middle - 1;
    else
      low = middle + 1;
  }
  return AOC_Output{0};
}

#include "../exec.hpp"