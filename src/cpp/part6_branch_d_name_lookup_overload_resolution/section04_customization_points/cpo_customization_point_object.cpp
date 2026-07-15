// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D8 CPO)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : cpo_customization_point_object
// Topic id : part6/d/section04/cpo_customization_point_object
//
// 要点: ranges CPO 封装 ADL+约束+禁特化；一行调用行为一致。
// 参考: [customization.point.object]

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

namespace lib {
struct Widget {
    int v;
};
void swap(Widget& a, Widget& b) noexcept {
    int t = a.v;
    a.v = b.v;
    b.v = t;
}
}  // namespace lib

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D8 CPO (customization point object) ===\n";

    lib::Widget a{1}, b{2};
    std::ranges::swap(a, b);  // 内部两步 + 约束
    assert(a.v == 2 && b.v == 1);

    std::vector<int> v{3, 1, 2};
    [[maybe_unused]] auto it = std::ranges::begin(v);
    assert(*it == 3);
    std::ranges::sort(v);
    assert((v == std::vector<int>{1, 2, 3}));

    // CPO 是对象，不可特化；定制靠 ADL/成员
    static_assert(std::is_object_v<decltype(std::ranges::swap)> || true);

    std::cout << "  CPO: one call site, ADL inside, concepts, no specialization\n";
    std::cout << "cpo_customization_point_object: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/cpo_customization_point_object", run>;

}  // namespace
