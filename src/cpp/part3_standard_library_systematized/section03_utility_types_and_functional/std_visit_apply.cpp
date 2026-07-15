// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : std_visit_apply
// Topic id : part3/section03/std_visit_apply
//
// Covers: std::visit and std::apply

#include "learn/topic_registry.hpp"

#include <string>
#include <tuple>
#include <type_traits>
#include <variant>

namespace {

void demo_basics() {
    std::variant<int, std::string> v = 5;
    int got = 0;
    std::visit(
        [&](auto&& x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, int>) {
                got = x;
            }
        },
        v);
    LEARN_CHECK(got == 5);
}

void demo_intermediate() {
    auto t = std::make_tuple(2, 3);
    int prod = std::apply([](int a, int b) { return a * b; }, t);
    LEARN_CHECK(prod == 6);
}

void demo_expert() {
    std::variant<int, double> a = 1;
    std::variant<int, double> b = 2.5;
    double sum = 0;
    std::visit([&](auto x, auto y) { sum = static_cast<double>(x) + static_cast<double>(y); }, a, b);
    LEARN_CHECK(sum > 3.4 && sum < 3.6);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/std_visit_apply", run>;

}  // namespace
