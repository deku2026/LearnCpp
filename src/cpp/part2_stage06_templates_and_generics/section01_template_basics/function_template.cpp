// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : function_template
// Topic id : part2/stage06/section01/function_template
//
// Covers: function templates, monomorphization, deduction vs explicit args

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>

namespace {

template <typename T>
T my_max(T a, T b) {
    return a > b ? a : b;
}

template <typename T, typename U>
auto add_mixed(T a, U b) -> decltype(a + b) {
    return a + b;
}

void demo_basics() {
    LEARN_CHECK(my_max(3, 5) == 5);
    LEARN_CHECK(my_max(2.5, 1.5) == 2.5);
    LEARN_CHECK(my_max<std::string>("a", "b") == "b");
}

void demo_intermediate() {
    LEARN_CHECK(add_mixed(3, 2.5) == 5.5);
    static_assert(std::is_same_v<decltype(my_max(1, 2)), int>);
    static_assert(std::is_same_v<decltype(my_max(1.0, 2.0)), double>);
}

void demo_expert() {
    // Explicit instantiation force: same template, different monomorphs.
    LEARN_CHECK(my_max<int>(3, 5) == 5);
    LEARN_CHECK(my_max<double>(3, 5) == 5.0);

    const int a = 10;
    const int b = 20;
    LEARN_CHECK(my_max(a, b) == 20);

    auto by_value = [](auto x, auto y) { return my_max(x, y); };
    LEARN_CHECK(by_value(std::string{"zz"}, std::string{"aa"}) == "zz");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/function_template", run>;

}  // namespace
