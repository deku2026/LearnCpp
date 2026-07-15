// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section09_customization_points
// Item     : cpo_begin_end_swap
// Topic id : part2/stage10/section09_customization_points/cpo_begin_end_swap
//
// Covers: CPO begin/end/swap two-step customization

#include "learn/topic_registry.hpp"

#include <ranges>
#include <utility>
#include <vector>

namespace mine {

struct Box {
    std::vector<int> data{1, 2, 3};
};

// Customization: ADL begin/end
auto begin(Box& b) {
    return b.data.begin();
}
auto end(Box& b) {
    return b.data.end();
}
auto begin(const Box& b) {
    return b.data.begin();
}
auto end(const Box& b) {
    return b.data.end();
}

void swap(Box& a, Box& b) noexcept {
    a.data.swap(b.data);
}

}  // namespace mine

namespace {

void demo_basics() {
    mine::Box b;
    LEARN_CHECK(std::ranges::size(b) == 3);
    LEARN_CHECK(*std::ranges::begin(b) == 1);
}

void demo_intermediate() {
    int a = 1;
    int b = 2;
    using std::swap;
    swap(a, b);  // two-step: ADL + std fallback
    LEARN_CHECK(a == 2 && b == 1);
}

void demo_expert() {
    mine::Box x;
    mine::Box y;
    y.data = {9, 9};
    using std::swap;
    swap(x, y);  // finds mine::swap via ADL
    LEARN_CHECK(x.data[0] == 9);
    LEARN_CHECK(y.data[0] == 1);
    // ranges::begin is a CPO: prefers member, then ADL
    LEARN_CHECK(std::ranges::begin(x) != std::ranges::end(x));
}

}  // namespace

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section09_customization_points/cpo_begin_end_swap", run>;

}  // namespace
