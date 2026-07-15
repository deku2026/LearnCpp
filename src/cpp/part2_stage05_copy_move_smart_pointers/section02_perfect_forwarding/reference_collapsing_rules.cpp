// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section02_perfect_forwarding
// Item     : reference_collapsing_rules
// Topic id : part2/stage05/section02/reference_collapsing_rules
//
// Covers: reference collapsing: & + anything -> &, && + && -> &&

#include "learn/topic_registry.hpp"

#include <cassert>
#include <type_traits>
#include <utility>

namespace {

template <class T>
using add_rref = T&&;

void demo_basics() {
    using A = int&;
    using B = int&&;
    static_assert(std::is_same_v<A&, int&>);
    static_assert(std::is_same_v<A&&, int&>);
    static_assert(std::is_same_v<B&, int&>);
    static_assert(std::is_same_v<B&&, int&&>);
}

void demo_intermediate() {
    // Collapsing via typedef / alias
    static_assert(std::is_same_v<add_rref<int&>, int&>);
    static_assert(std::is_same_v<add_rref<int&&>, int&&>);
    static_assert(std::is_same_v<add_rref<int>, int&&>);

    // const participates (via alias, not raw T&&& spellings)
    using CR = const int&&;
    static_assert(std::is_same_v<CR&, const int&>);
    static_assert(std::is_same_v<add_rref<const int&>, const int&>);
}

template <class T>
struct Wrapper {
    using ref = T&&;
};

void demo_expert() {
    static_assert(std::is_same_v<Wrapper<int&>::ref, int&>);
    static_assert(std::is_same_v<Wrapper<int&&>::ref, int&&>);
    static_assert(std::is_same_v<Wrapper<int>::ref, int&&>);

    // Perfect-forwarding deduction relies on collapsing.
    auto f = [](auto&& x) -> decltype(auto) { return std::forward<decltype(x)>(x); };
    int v = 10;
    int& lr = f(v);
    assert(&lr == &v);
    int&& rr = f(20);
    assert(rr == 20);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section02/reference_collapsing_rules", run>;

}  // namespace
