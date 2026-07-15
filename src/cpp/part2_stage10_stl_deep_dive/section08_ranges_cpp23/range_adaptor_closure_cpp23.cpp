// LearnCpp topic example
// Doc      : part2-stage10-stl-deep-dive.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section08_ranges_cpp23
// Item     : range_adaptor_closure_cpp23
// Topic id : part2/stage10/section08_ranges_cpp23/range_adaptor_closure_cpp23
//
// Covers: range_adaptor_closure user-defined pipe C++23 P2387

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>
#include <version>

namespace {

#if defined(__cpp_lib_ranges) && __cplusplus >= 202302L
struct times_n : std::ranges::range_adaptor_closure<times_n> {
    int n;
    explicit times_n(int n_) : n(n_) {}
    template <std::ranges::viewable_range R>
    auto operator()(R&& r) const {
        return std::forward<R>(r) | std::views::transform([n = n](auto x) { return x * n; });
    }
};
#endif

void demo_basics() {
#if defined(__cpp_lib_ranges) && __cplusplus >= 202302L
    std::vector<int> v{1, 2, 3};
    auto r = v | times_n{10};
    std::vector<int> out(r.begin(), r.end());
    LEARN_CHECK((out == std::vector<int>{10, 20, 30}));
#else
    std::vector<int> v{1, 2, 3};
    auto r = v | std::views::transform([](int x) { return x * 10; });
    std::vector<int> out(r.begin(), r.end());
    LEARN_CHECK((out == std::vector<int>{10, 20, 30}));
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_ranges) && __cplusplus >= 202302L
    auto r = std::views::iota(1, 4) | times_n{2} | std::views::filter([](int x) { return x > 2; });
    LEARN_CHECK(std::ranges::distance(r) == 2);
#else
    LEARN_CHECK(true);
#endif
}

void demo_expert() {
#if defined(__cpp_lib_ranges) && __cplusplus >= 202302L
    times_n t{3};
    std::vector<int> v{1, 1, 1};
    auto r = t(v);
    int sum = 0;
    for (int x : r) {
        sum += x;
    }
    LEARN_CHECK(sum == 9);
#else
    LEARN_CHECK(true);
#endif
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage10/section08_ranges_cpp23/range_adaptor_closure_cpp23", run>;

}  // namespace
