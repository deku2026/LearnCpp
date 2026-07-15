// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : sfinae_substitution_failure
// Topic id : part2/stage06/section04/sfinae_substitution_failure
//
// Covers: SFINAE — invalid substitution discards overload, not hard error

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <utility>

namespace {

template <typename T>
auto has_c_str_impl(int) -> decltype(std::declval<T>().c_str(), std::true_type{});

template <typename T>
std::false_type has_c_str_impl(...);

template <typename T>
constexpr bool has_c_str_v = decltype(has_c_str_impl<T>(0))::value;

template <typename T>
auto serialize(const T& x, int) -> decltype(x.c_str(), std::string{}) {
    return std::string{x.c_str()};
}

template <typename T>
std::string serialize(const T& x, ...) {
    return std::to_string(x);
}

void demo_basics() {
    static_assert(has_c_str_v<std::string>);
    static_assert(!has_c_str_v<int>);
    assert(has_c_str_v<std::string>);
}

void demo_intermediate() {
    assert(serialize(std::string{"hi"}, 0) == "hi");
    assert(serialize(42, 0) == "42");
}

void demo_expert() {
    // Prefer int overload; ... is fallback when substitution fails.
    struct WithCStr {
        const char* c_str() const { return "ok"; }
    };
    assert(serialize(WithCStr{}, 0) == "ok");
    assert(serialize(7, 0) == "7");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/sfinae_substitution_failure", run>;

}  // namespace
