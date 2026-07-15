// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section03_generic_and_compile_time
// Item     : cpo_customization_point
// Topic id : part2/stage15/section03/cpo_customization_point
//
// Covers: CPO-style customization point

#include "learn/topic_registry.hpp"

#include <cassert>
#include <utility>

namespace {

namespace lib {
namespace detail {
template <class T>
void tag_invoke_draw(const T& t) = delete;

struct draw_fn {
    template <class T>
    int operator()(const T& t) const {
        return tag_invoke_draw(t);
    }
};
}  // namespace detail
inline constexpr detail::draw_fn draw{};
}  // namespace lib

struct Circle {
    int r;
};
int tag_invoke_draw(const Circle& c) {
    return c.r * c.r;
}

struct Box {
    int w;
};
int tag_invoke_draw(const Box& b) {
    return b.w;
}

void demo_basics() {
    assert(lib::draw(Circle{3}) == 9);
}

void demo_intermediate() {
    assert(lib::draw(Box{4}) == 4);
}

void demo_expert() {
    // User types opt in via free function associated with the type.
    Circle c{2};
    Box b{5};
    assert(lib::draw(c) + lib::draw(b) == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section03/cpo_customization_point", run>;

}  // namespace
