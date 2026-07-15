// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : partial_template_specialization
// Topic id : part2/stage06/section01/partial_template_specialization
//
// Covers: partial specialization (pointer, pair-like, more specialized wins)

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

template <typename T>
struct Strip {
    using type = T;
    static constexpr int kind = 0;
};

template <typename T>
struct Strip<T*> {
    using type = T;
    static constexpr int kind = 1;
};

template <typename T>
struct Strip<const T> {
    using type = T;
    static constexpr int kind = 2;
};

template <typename T, typename U>
struct PairTag {
    static constexpr int kind = 0;
};

template <typename T>
struct PairTag<T, T> {
    static constexpr int kind = 1;  // same types
};

template <typename T>
struct PairTag<T*, T*> {
    static constexpr int kind = 2;  // both pointers, more specialized than <T,T>
};

void demo_basics() {
    static_assert(Strip<int>::kind == 0);
    static_assert(Strip<int*>::kind == 1);
    static_assert(std::is_same_v<Strip<int*>::type, int>);
    LEARN_CHECK(Strip<int*>::kind == 1);
}

void demo_intermediate() {
    static_assert(Strip<const int>::kind == 2);
    static_assert(std::is_same_v<Strip<const int>::type, int>);

    static_assert(PairTag<int, double>::kind == 0);
    static_assert(PairTag<int, int>::kind == 1);
}

void demo_expert() {
    static_assert(PairTag<int*, int*>::kind == 2);

    using S = Strip<const char*>;
    // const char* matches T* with T = const char (pointer partial), not const T.
    static_assert(S::kind == 1);
    static_assert(std::is_same_v<S::type, const char>);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/partial_template_specialization", run>;

}  // namespace
