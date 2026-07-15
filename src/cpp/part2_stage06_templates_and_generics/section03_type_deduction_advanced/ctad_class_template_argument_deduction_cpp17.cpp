// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : ctad_class_template_argument_deduction_cpp17
// Topic id : part2/stage06/section03/ctad_class_template_argument_deduction_cpp17
//
// Covers: CTAD for pair/array/custom class templates (C++17)

#include "learn/topic_registry.hpp"

#include <array>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <typename T, typename U>
struct Pair {
    T first;
    U second;
    Pair(T a, U b) : first(std::move(a)), second(std::move(b)) {}
};

void demo_basics() {
    std::pair p{1, 2.5};  // CTAD -> pair<int,double>
    LEARN_CHECK(p.first == 1);
    LEARN_CHECK(p.second == 2.5);
    static_assert(std::is_same_v<decltype(p), std::pair<int, double>>);
}

void demo_intermediate() {
    std::array a{1, 2, 3};  // array<int,3>
    LEARN_CHECK(a.size() == 3);
    LEARN_CHECK(a[1] == 2);
    static_assert(std::is_same_v<decltype(a), std::array<int, 3>>);

    std::vector v{std::string{"x"}, std::string{"y"}};
    LEARN_CHECK(v.size() == 2);
    static_assert(std::is_same_v<decltype(v), std::vector<std::string>>);
}

void demo_expert() {
    Pair pr{std::string{"k"}, 42};
    LEARN_CHECK(pr.first == "k");
    LEARN_CHECK(pr.second == 42);
    static_assert(std::is_same_v<decltype(pr), Pair<std::string, int>>);

    // Explicit template args still work and override CTAD.
    std::pair<long, long> q{1, 2};
    LEARN_CHECK(q.first == 1L);
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
    ::learn::topic<"part2/stage06/section03/ctad_class_template_argument_deduction_cpp17", run>;

}  // namespace
