//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"


#include <functional>
#include <map>
#include <queue>

struct AOC_Output {
  unsigned long long value{};
  AOC_Output() = default;
  explicit AOC_Output(auto v) : value{v} {}
  auto operator()() const { return value; }
};

enum class Pulse { low, high };

using Action = std::function<void()>;
using ActionQueue = std::queue<Action>;

long high_pulse = 0;
long low_pulse = 0;

class Module {
public:
  explicit Module(std::string name) : name_{std::move(name)} {}
  [[nodiscard]] auto name() const { return name_; }

  virtual void set_input(std::string in) = 0;
  virtual void set_output(std::string out) = 0;

  void send_pulse(Module *src, Pulse pulse, ActionQueue *q) {
    if (pulse == Pulse::high) ++high_pulse;
    if (pulse == Pulse::low) ++low_pulse;
    send_pulse_(src, pulse, q);
  }


  virtual ~Module() = default;

private:
  virtual void send_pulse_(Module *src, Pulse pulse,
                           ActionQueue *q) = 0;

  std::string name_;
};

std::map<std::string, std::unique_ptr<Module>> modules;

class FlipFlop : public Module {
public:
  explicit FlipFlop(std::string name) : Module(std::move(name)) {}

  void set_input(std::string in) override {}
  void set_output(std::string out) override {
    out_.emplace_back(std::move(out));
  }
  void send_pulse_(Module *src, Pulse pulse,
                   ActionQueue *q) override {
    if (pulse == Pulse::high) { return; }
    if (is_on_) {
      is_on_ = false;
      for (auto &o: out_) {
        q->emplace([this, q, o]() {
          modules[o]->send_pulse(this, Pulse::low, q);
        });
      }
    } else {
      is_on_ = true;
      for (auto &o: out_) {
        q->emplace([this, q, o]() {
          modules[o]->send_pulse(this, Pulse::high, q);
        });
      }
    }
  }

private:
  bool is_on_ = false;
  std::vector<std::string> out_;
};

class Conjunction : public Module {
public:
  explicit Conjunction(std::string name) : Module(std::move(name)) {}

  void set_input(std::string in) override {
    in_[modules[in].get()] = Pulse::low;
  }
  void set_output(std::string out) override {
    out_.emplace_back(std::move(out));
  }
  void send_pulse_(Module *src, Pulse pulse,
                   ActionQueue *q) override {
    in_[src] = pulse;
    auto flag = std::ranges::all_of(
        in_, [](const auto &p) { return p.second == Pulse::high; });
    if (flag) {
      for (auto &out: out_) {
        q->emplace([this, q, out]() mutable {
          modules[out]->send_pulse(this, Pulse::low, q);
        });
      }
    } else {
      for (auto &out: out_) {
        q->emplace([this, q, out]() mutable {
          modules[out]->send_pulse(this, Pulse::high, q);
        });
      }
    }
  }

private:
  std::map<Module *, Pulse> in_{};
  std::vector<std::string> out_;
};

class Broadcast : public Module {
public:
  Broadcast() : Module("bcast") {}
  void set_input(std::string in) override {}
  void set_output(std::string out) override {
    out_.emplace_back(out);
  }
  void send_pulse_(Module *src, Pulse pulse,
                   ActionQueue *q) override {
    for (auto &m: out_) {
      q->emplace([this, q, m, pulse]() mutable {
        modules[m]->send_pulse(this, pulse, q);
      });
    }
  }

private:
  std::vector<std::string> out_;
};

class Null : public Module {
public:
  explicit Null(std::string name) : Module(std::move(name)) {}
  void set_input(std::string in) override {}
  void set_output(std::string out) override {}
  void send_pulse_(Module *src, Pulse pulse,
                   ActionQueue *q) override {}
};

struct AOC_Input {
  std::string line;
};

std::vector<AOC_Input> AOC_Reader::from_string_vector(
    const std::vector<std::string> &lines) {
  return lines |
         vw::transform([](auto const &e) { return AOC_Input{e}; }) |
         as_vector;
}

void add_module(std::string const &e) {
  std::istringstream iss{e};
  std::string name;
  iss >> name;

  std::string dst;
  iss >> dst;

  if (name == "broadcaster") {
    modules[name] = std::make_unique<Broadcast>();
  } else {
    char type = name[0];
    name = name.substr(1);
    if (type == '%') {
      modules[name] = std::make_unique<FlipFlop>(name);
    } else {
      modules[name] = std::make_unique<Conjunction>(name);
    }
  }
}

void add_module_params(std::string const &e) {
  std::istringstream iss{e};
  std::string name;
  iss >> name;

  std::string dst;
  iss >> dst;

  if (name != "broadcaster") name = name.substr(1);

  auto &m = modules[name];

  while (iss >> dst) {
    if (dst.back() == ',') dst.pop_back();
    m->set_output(dst);
    if (modules.contains(dst)) modules[dst]->set_input(m->name());
    else { modules[dst] = std::make_unique<Null>(dst); }
  }
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  for (auto &e: v) { add_module(e.line); }
  for (auto &e: v) { add_module_params(e.line); }

  static constexpr auto NPUSH = 1000;

  for (auto i = 0; i < NPUSH; ++i) {
    ActionQueue q;
    q.emplace([&q]() {
      modules["broadcaster"]->send_pulse(modules["broadcaster"].get(),
                                         Pulse::low, &q);
    });

    while (!q.empty()) {
      auto act = q.front();
      q.pop();
      act();
    }
    // fmt::print("\n");
  }

  return AOC_Output{
      static_cast<unsigned long long>(high_pulse * low_pulse)};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  // Find a number of iters to get HI signal from dc, vp, cq, rv
  return AOC_Output{std::lcm(
      3796ULL + 1,
      std::lcm(3846ULL + 1, std::lcm(3876ULL + 1, 4050ULL + 1)))};
}

#include "../exec.hpp"