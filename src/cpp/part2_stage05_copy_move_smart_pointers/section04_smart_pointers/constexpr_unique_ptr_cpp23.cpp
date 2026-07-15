// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : constexpr_unique_ptr_cpp23
// Topic id : part2/stage05/section04/constexpr_unique_ptr_cpp23
//
// Covers: C++23 constexpr unique_ptr / make_unique in constant evaluation

#include "learn/topic_registry.hpp"

#include <memory>
#include <type_traits>
#include <version>

namespace {

#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
constexpr int compute_with_unique_ptr() {
    auto p = std::make_unique<int>(42);
    return *p;
}
#endif

void demo_basics() {
    auto p = std::make_unique<int>(42);
    LEARN_CHECK(p != nullptr);
    LEARN_CHECK(*p == 42);
}

void demo_intermediate() {
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    constexpr int v = compute_with_unique_ptr();
    static_assert(v == 42);
    LEARN_CHECK(v == 42);
#else
    // Portable fallback when constexpr unique_ptr is unavailable.
    const int v = [] {
        auto p = std::make_unique<int>(42);
        return *p;
    }();
    LEARN_CHECK(v == 42);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_constexpr_memory) && __cpp_lib_constexpr_memory >= 202202L
    constexpr auto sum = []() constexpr {
        auto a = std::make_unique<int>(3);
        auto b = std::make_unique<int>(4);
        return *a + *b;
    }();
    static_assert(sum == 7);
    LEARN_CHECK(sum == 7);
#else
    auto a = std::make_unique<int>(3);
    auto b = std::make_unique<int>(4);
    LEARN_CHECK(*a + *b == 7);
#endif

#if defined(__cpp_lib_constexpr_memory)
    static_assert(__cpp_lib_constexpr_memory >= 201907L || __cpp_lib_constexpr_memory < 201907L);
#endif
    static_assert(std::is_same_v<decltype(std::make_unique<int>(1)), std::unique_ptr<int>>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/constexpr_unique_ptr_cpp23", run>;

}  // namespace
