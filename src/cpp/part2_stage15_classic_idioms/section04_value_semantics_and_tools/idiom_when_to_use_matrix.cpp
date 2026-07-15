// LearnCpp topic
// Doc      : 第2部分-阶段15-C++经典惯用法.md · 步骤 7（何时不用矩阵）
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : idiom_when_to_use_matrix
// Topic id : part2/stage15/section04/idiom_when_to_use_matrix
//
// Idioms are tools: overuse is a smell. This topic encodes the doc's
// "when NOT to use" table as runnable decision notes + tiny probes.
// Refs     : C++ Core Guidelines; stage 15 step 7

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace {

// Minimal probes so the matrix is not only comments.

// strong typedef: good when mixing width/height ints; overkill for local loop i
struct Width {
    int v{};
};
struct Height {
    int v{};
};
int area(Width w, Height h) {
    return w.v * h.v;
}

// optional: good for "present or not"; use expected when reason matters
std::optional<int> find_even(const std::vector<int>& v) {
    for (int x : v) {
        if (x % 2 == 0) {
            return x;
        }
    }
    return std::nullopt;
}

// builder: justified when many optional fields; 2–3 args → plain ctor
struct Config {
    std::string name;
    int port = 80;
    bool tls = false;
};

struct ConfigBuilder {
    Config c_;

    ConfigBuilder& name(std::string n) {
        c_.name = std::move(n);
        return *this;
    }
    ConfigBuilder& port(int p) {
        c_.port = p;
        return *this;
    }
    ConfigBuilder& tls(bool t) {
        c_.tls = t;
        return *this;
    }
    Config build() const { return c_; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [idiom_when_to_use_matrix] step 7 — when NOT to use ===\n";

    // Probe: strong typedef catches argument swap at the type level
    assert(area(Width{3}, Height{4}) == 12);
    // area(Height{4}, Width{3}); // would not compile if call sites used types

    assert(find_even({1, 3, 4, 5}) == 4);
    assert(!find_even({1, 3, 5}));

    const Config cfg = ConfigBuilder{}.name("svc").port(443).tls(true).build();
    assert(cfg.port == 443 && cfg.tls && cfg.name == "svc");

    std::cout << "\n| idiom              | ⚠️ when NOT to use (prefer simpler)          |\n";
    std::cout << "|--------------------|---------------------------------------------|\n";
    std::cout << "| copy-and-swap      | hot assignment path; write copy/move assign |\n";
    std::cout << "| pimpl              | tiny hot types (extra alloc + indirection)  |\n";
    std::cout << "| scope guard        | when a dedicated RAII type already fits     |\n";
    std::cout << "| NVI                | trivial interfaces (public virtual is fine) |\n";
    std::cout << "| CRTP               | need runtime polymorphism → virtual/erase   |\n";
    std::cout << "| type erasure       | closed set known → variant                  |\n";
    std::cout << "| tag dispatch       | prefer Concepts / if constexpr (C++20+)     |\n";
    std::cout << "| detection idiom    | prefer Concepts + requires                  |\n";
    std::cout << "| SFINAE             | new code: if constexpr + Concepts           |\n";
    std::cout << "| policy-based       | policy explosion / readability collapse     |\n";
    std::cout << "| strong typedef     | every local int (over-engineering)          |\n";
    std::cout << "| builder            | 2–3 args: plain constructor                 |\n";
    std::cout << "| expr. templates    | app code — leave to Eigen-class libraries   |\n";
    std::cout << "| optional           | need failure *reason* → expected            |\n";
    std::cout << "| recursive lambda   | deep recursion → named function             |\n";

    std::cout << "\nmeta-rule: if you cannot answer \"what problem does this solve?\",\n"
                 "do not apply the idiom — clarity beats looking advanced.\n";

    // Cross-links to concrete demos in this stage
    std::cout << "demos: copy_and_swap / pimpl / nvi / crtp / type_erasure /\n"
                 "       detection / policy_based / strong_typedef / named_parameter_builder\n";

    std::cout << "[idiom_when_to_use_matrix] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/idiom_when_to_use_matrix", run>;

}  // namespace
