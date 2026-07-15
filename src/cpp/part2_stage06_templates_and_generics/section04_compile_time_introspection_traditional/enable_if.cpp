// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : enable_if
// Topic id : part2/stage06/section04/enable_if
//
// Covers: std::enable_if / enable_if_t for overload selection (pre-concepts)

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>

namespace {

template <typename T>
std::enable_if_t<std::is_integral_v<T>, int> classify(T) {
    return 1;
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, int> classify(T) {
    return 2;
}

template <typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, int> classify(T) {
    return 3;
}

template <typename T, typename = void>
struct HasSize : std::false_type {};

template <typename T>
struct HasSize<T, std::enable_if_t<std::is_same_v<decltype(std::declval<T>().size()), std::size_t>>> : std::true_type {
};

void demo_basics() {
    LEARN_CHECK(classify(42) == 1);
    LEARN_CHECK(classify(3.14) == 2);
    LEARN_CHECK(classify(std::string{"x"}) == 3);
}

void demo_intermediate() {
    static_assert(HasSize<std::string>::value);
    struct NoSize {};
    static_assert(!HasSize<NoSize>::value);
}

void demo_expert() {
    // Return-type SFINAE style.
    auto only_integral = [](auto x) -> std::enable_if_t<std::is_integral_v<decltype(x)>, int> {
        return static_cast<int>(x) * 2;
    };
    LEARN_CHECK(only_integral(21) == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/enable_if", run>;

}  // namespace
