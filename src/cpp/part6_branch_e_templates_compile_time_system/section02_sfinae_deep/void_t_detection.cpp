// LearnCpp topic example
// Doc      : part6-branch-e-templates-compile-time.md
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : void_t_detection
// Topic id : part6/e/section02/void_t_detection
//
// Covers: void_t detection idiom

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>

namespace {

template <class, class = void>
struct has_type_member : std::false_type {};

template <class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

struct Yes {
    using type = int;
};
struct No {};

void demo_basics() {
    static_assert(has_type_member<Yes>::value);
    static_assert(!has_type_member<No>::value);
}

void demo_intermediate() {
    assert(has_type_member<Yes>::value);
    assert(!has_type_member<No>::value);
}

void demo_expert() {
    static_assert(std::is_void_v<std::void_t<int, double>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/void_t_detection", run>;

}  // namespace
