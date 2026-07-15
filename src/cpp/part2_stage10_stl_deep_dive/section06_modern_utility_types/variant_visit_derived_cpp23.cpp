// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : variant_visit_derived_cpp23
// Topic id : part2/stage10/section06_modern_utility_types/variant_visit_derived_cpp23
//
// Covers: variant visit enhancements / derived visitors C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <variant>
#include <version>

namespace {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

struct BaseVisitor {
    int operator()(int x) const { return x; }
};

struct DerivedVisitor : BaseVisitor {
    using BaseVisitor::operator();
    int operator()(const std::string& s) const { return static_cast<int>(s.size()); }
};

void demo_basics() {
    std::variant<int, std::string> v = 5;
    LEARN_CHECK(std::visit(DerivedVisitor{}, v) == 5);
}

void demo_intermediate() {
    std::variant<int, std::string> v = std::string{"xy"};
    LEARN_CHECK(std::visit(DerivedVisitor{}, v) == 2);
}

void demo_expert() {
    // C++23: visit may be more constexpr-friendly; overloaded remains the common idiom
    std::variant<int, std::string, double> v = 1.5;
    const auto r = std::visit(overloaded{
                                  [](int x) { return static_cast<double>(x); },
                                  [](const std::string& s) { return static_cast<double>(s.size()); },
                                  [](double d) { return d; },
                              },
                              v);
    LEARN_CHECK(r == 1.5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section06_modern_utility_types/variant_visit_derived_cpp23", run>;

}  // namespace
