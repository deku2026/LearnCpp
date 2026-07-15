// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : if_constexpr_cpp17
// Topic id : part2/stage06/section04/if_constexpr_cpp17
//
// Covers: if constexpr discards untaken branch at compile time

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <vector>

namespace {

template <typename T>
auto describe(const T& x) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<int>(x) * 2;
    } else if constexpr (std::is_same_v<T, std::string>) {
        return static_cast<int>(x.size());
    } else {
        return -1;
    }
}

template <typename T>
int safe_size(const T& x) {
    if constexpr (requires { x.size(); }) {
        return static_cast<int>(x.size());
    } else {
        return 0;
    }
}

void demo_basics() {
    assert(describe(21) == 42);
    assert(describe(std::string{"abcd"}) == 4);
    assert(describe(3.14) == -1);
}

void demo_intermediate() {
    assert((safe_size(std::vector<int>{1, 2, 3}) == 3));
    assert(safe_size(42) == 0);
    assert(safe_size(std::string{"xy"}) == 2);
}

void demo_expert() {
    auto print_type_kind = [](auto v) {
        using T = decltype(v);
        if constexpr (std::is_pointer_v<T>) {
            return 1;
        } else if constexpr (std::is_reference_v<T>) {
            return 2;
        } else {
            return 0;
        }
    };
    int x = 0;
    assert(print_type_kind(x) == 0);
    assert(print_type_kind(&x) == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/if_constexpr_cpp17", run>;

}  // namespace
