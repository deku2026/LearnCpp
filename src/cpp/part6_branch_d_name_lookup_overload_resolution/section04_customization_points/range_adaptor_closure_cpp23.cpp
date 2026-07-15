// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D9 range_adaptor_closure)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : range_adaptor_closure_cpp23
// Topic id : part6/d/section04/range_adaptor_closure_cpp23
//
// 要点: 继承 range_adaptor_closure 即可接 | 管道。
// 参考: https://en.cppreference.com/w/cpp/ranges/range_adaptor_closure

#include "learn/topic_registry.hpp"

#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace {

#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
struct Times2 : std::ranges::range_adaptor_closure<Times2> {
    template <std::ranges::viewable_range R>
    auto operator()(R&& r) const {
        return std::views::transform(std::forward<R>(r), [](auto x) { return x * 2; });
    }
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D9 range_adaptor_closure (C++23) ===\n";

#if defined(__cpp_lib_ranges) && __cpp_lib_ranges >= 202202L
    std::vector<int> v{1, 2, 3};
    std::vector<int> out;
    for (int x : v | Times2{}) out.push_back(x);
    assert((out == std::vector<int>{2, 4, 6}));

    out.clear();
    for (int x : v | Times2{} | std::views::filter([](int x) { return x > 2; })) {
        out.push_back(x);
    }
    assert((out == std::vector<int>{4, 6}));
    std::cout << "  custom adaptor pipes like standard views\n";
#else
    // 回退：手动 transform
    std::vector<int> v{1, 2, 3};
    std::vector<int> out;
    for (int x : v) out.push_back(x * 2);
    assert((out == std::vector<int>{2, 4, 6}));
    std::cout << "  range_adaptor_closure not available; transform fallback\n";
#endif

    std::cout << "range_adaptor_closure_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/range_adaptor_closure_cpp23", run>;

}  // namespace
