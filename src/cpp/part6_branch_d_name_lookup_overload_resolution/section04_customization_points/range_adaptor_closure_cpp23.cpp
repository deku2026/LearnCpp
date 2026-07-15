// LearnCpp topic example
// Doc      : part6-branch-d-name-lookup-adl-overload.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : range_adaptor_closure_cpp23
// Topic id : part6/d/section04/range_adaptor_closure_cpp23
//
// Covers: range adaptor closure objects (C++20/23 pipeline)

#include "learn/topic_registry.hpp"

#include <ranges>
#include <vector>

namespace {

void demo_basics() {
    std::vector<int> v{1, 2, 3, 4};
    auto even = v | std::views::filter([](int x) { return x % 2 == 0; });
    int sum = 0;
    for (int x : even) sum += x;
    LEARN_CHECK(sum == 6);
}

void demo_intermediate() {
    std::vector<int> v{1, 2, 3};
    auto squared = v | std::views::transform([](int x) { return x * x; });
    int sum = 0;
    for (int x : squared) sum += x;
    LEARN_CHECK(sum == 14);
}

void demo_expert() {
#if defined(__cpp_lib_ranges_to_container) && __cpp_lib_ranges_to_container >= 202202L
    std::vector<int> v{1, 2, 3};
    auto out = v | std::views::transform([](int x) { return x + 1; }) | std::ranges::to<std::vector>();
    LEARN_CHECK(out.size() == 3);
    LEARN_CHECK(out[0] == 2);
#else
    std::vector<int> out;
    for (int x : std::vector<int>{1, 2, 3} | std::views::transform([](int n) { return n + 1; })) {
        out.push_back(x);
    }
    LEARN_CHECK(out[0] == 2);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/range_adaptor_closure_cpp23", run>;

}  // namespace
