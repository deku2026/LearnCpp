// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : swap_two_step_idiom
// Topic id : part6/d/section04/swap_two_step_idiom
//
// Covers: using std::swap; swap(a,b); two-step idiom

#include "learn/topic_registry.hpp"

#include <utility>

namespace {

namespace ns {
struct Token {
    int id = 0;
};
void swap(Token& a, Token& b) noexcept {
    int t = a.id;
    a.id = b.id;
    b.id = t;
}
}  // namespace ns

template <class T>
void generic_swap(T& a, T& b) {
    using std::swap;
    swap(a, b);
}

void demo_basics() {
    ns::Token a{1}, b{2};
    generic_swap(a, b);
    LEARN_CHECK(a.id == 2 && b.id == 1);
}

void demo_intermediate() {
    int x = 3, y = 4;
    generic_swap(x, y);
    LEARN_CHECK(x == 4 && y == 3);
}

void demo_expert() {
    ns::Token a{9}, b{8};
    using std::swap;
    swap(a, b);
    LEARN_CHECK(a.id == 8 && b.id == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/swap_two_step_idiom", run>;

}  // namespace
