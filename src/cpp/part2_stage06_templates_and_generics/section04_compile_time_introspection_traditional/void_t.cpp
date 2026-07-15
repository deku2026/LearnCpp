// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : void_t
// Topic id : part2/stage06/section04/void_t
//
// Covers: std::void_t detection idiom for members/operators

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

template <typename, typename = void>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type {};

template <typename, typename = void>
struct has_plus : std::false_type {};

template <typename T>
struct has_plus<T, std::void_t<decltype(std::declval<T>() + std::declval<T>())>> : std::true_type {};

template <typename, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

template <typename T>
constexpr bool has_size_v = has_size<T>::value;

void demo_basics() {
    static_assert(has_size_v<std::string>);
    static_assert(has_size_v<std::vector<int>>);
    static_assert(!has_size_v<int>);
    LEARN_CHECK(has_size_v<std::string>);
}

void demo_intermediate() {
    static_assert(has_plus<int>::value);
    static_assert(has_plus<std::string>::value);
    struct NoPlus {};
    static_assert(!has_plus<NoPlus>::value);
}

void demo_expert() {
    static_assert(has_value_type<std::vector<int>>::value);
    static_assert(!has_value_type<int>::value);
    static_assert(std::is_same_v<std::vector<int>::value_type, int>);
    LEARN_CHECK(has_size_v<std::vector<char>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/void_t", run>;

}  // namespace
