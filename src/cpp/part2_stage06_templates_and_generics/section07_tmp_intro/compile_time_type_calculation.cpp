// LearnCpp topic example
// Doc      : part2-stage06-templates-and-generics.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section07_tmp_intro
// Item     : compile_time_type_calculation
// Topic id : part2/stage06/section07/compile_time_type_calculation
//
// Covers: type-level computation (conditional, promote, type lists)

#include "learn/topic_registry.hpp"

#include <type_traits>

namespace {

template <bool B, typename T, typename F>
struct Cond {
    using type = T;
};

template <typename T, typename F>
struct Cond<false, T, F> {
    using type = F;
};

template <bool B, typename T, typename F>
using Cond_t = typename Cond<B, T, F>::type;

template <typename T>
struct Promote {
    using type = Cond_t<std::is_integral_v<T>, long, T>;
};

template <typename T>
using Promote_t = typename Promote<T>::type;

template <typename... Ts>
struct TypeList {};

template <typename List>
struct Front;

template <typename H, typename... T>
struct Front<TypeList<H, T...>> {
    using type = H;
};

void demo_basics() {
    static_assert(std::is_same_v<Cond_t<true, int, double>, int>);
    static_assert(std::is_same_v<Cond_t<false, int, double>, double>);
}

void demo_intermediate() {
    static_assert(std::is_same_v<Promote_t<int>, long>);
    static_assert(std::is_same_v<Promote_t<double>, double>);
    Promote_t<int> x = 1L;
    LEARN_CHECK(x == 1L);
}

void demo_expert() {
    using L = TypeList<char, int, double>;
    static_assert(std::is_same_v<Front<L>::type, char>);
    using P = Promote_t<Front<L>::type>;
    static_assert(std::is_same_v<P, long>);  // char is integral -> long
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section07/compile_time_type_calculation", run>;

}  // namespace
