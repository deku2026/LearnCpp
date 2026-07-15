// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section05_pattern_matching_alternatives
// Item     : variant_visit_overload
// Topic id : part2/stage08/section05/variant_visit_overload
//
// Covers: std::variant + visit + overload pattern (pattern-matching stand-in)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <variant>

namespace {

template <class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

using Value = std::variant<int, double, std::string>;

std::string describe(const Value& v) {
    return std::visit(overload{
                          [](int x) { return std::string{"i:"} + std::to_string(x); },
                          [](double x) { return std::string{"d:"} + std::to_string(static_cast<int>(x)); },
                          [](const std::string& s) { return std::string{"s:"} + s; },
                      },
                      v);
}

int as_int(const Value& v) {
    return std::visit(overload{
                          [](int x) { return x; },
                          [](double x) { return static_cast<int>(x); },
                          [](const std::string& s) { return static_cast<int>(s.size()); },
                      },
                      v);
}

void demo_basics() {
    Value v = 42;
    assert(describe(v) == "i:42");
    v = std::string{"hi"};
    assert(describe(v) == "s:hi");
}

void demo_intermediate() {
    Value v = 3.9;
    assert(as_int(v) == 3);
    v = 10;
    assert(as_int(v) == 10);
    v = std::string{"abcd"};
    assert(as_int(v) == 4);
}

void demo_expert() {
    // index() / holds_alternative as matching helpers.
    Value v = 1;
    assert(std::holds_alternative<int>(v));
    assert(v.index() == 0);
    v = std::string{"x"};
    assert(std::get<std::string>(v) == "x");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section05/variant_visit_overload", run>;

}  // namespace
