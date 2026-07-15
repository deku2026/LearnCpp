// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : variant_visit_overload_idiom
// Topic id : part2/stage10/section06_modern_utility_types/variant_visit_overload_idiom
//
// Covers: variant visit with overloaded lambda idiom

#include "learn/topic_registry.hpp"

#include <string>
#include <variant>

namespace {

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void demo_basics() {
    std::variant<int, std::string> v = 10;
    int result = 0;
    std::visit(overloaded{
                   [&](int x) { result = x * 2; },
                   [&](const std::string& s) { result = static_cast<int>(s.size()); },
               },
               v);
    LEARN_CHECK(result == 20);
}

void demo_intermediate() {
    std::variant<int, std::string> v = std::string{"abc"};
    const auto r = std::visit(overloaded{
                                  [](int x) { return x; },
                                  [](const std::string& s) { return static_cast<int>(s.size()); },
                              },
                              v);
    LEARN_CHECK(r == 3);
}

void demo_expert() {
    std::variant<int, double, std::string> v = 2.5;
    const auto tag = std::visit(overloaded{
                                    [](int) { return 1; },
                                    [](double) { return 2; },
                                    [](const std::string&) { return 3; },
                                },
                                v);
    LEARN_CHECK(tag == 2);
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
    ::learn::topic<"part2/stage10/section06_modern_utility_types/variant_visit_overload_idiom", run>;

}  // namespace
