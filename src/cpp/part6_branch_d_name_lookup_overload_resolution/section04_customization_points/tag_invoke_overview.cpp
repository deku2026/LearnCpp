// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D9 tag_invoke)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : tag_invoke_overview
// Topic id : part6/d/section04/tag_invoke_overview
//
// 要点: tag_invoke 是库界统一定制点方案（未进标准）——单 CPO + tag 分派。
// 参考: P1895

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

// 教学用迷你 tag_invoke 机制（非 std）
struct tag_invoke_t {
    template <typename Tag, typename... Args>
    constexpr auto operator()(Tag tag, Args&&... args) const -> decltype(tag_invoke(tag, std::forward<Args>(args)...)) {
        return tag_invoke(tag, std::forward<Args>(args)...);
    }
};
inline constexpr tag_invoke_t tag_invoke_cpo{};

// tags
struct serialize_t {
    explicit serialize_t() = default;
};
inline constexpr serialize_t serialize{};

namespace geometry {
struct Point {
    int x, y;
};
// 定制：ADL 可见的 tag_invoke
std::string tag_invoke(serialize_t, const Point& p) {
    return "Point(" + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
}
}  // namespace geometry

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D9 tag_invoke overview (library pattern) ===\n";

    geometry::Point p{3, 4};
    // CPO 式：tag_invoke_cpo(serialize, p) → ADL tag_invoke(serialize, p)
    std::string s = tag_invoke_cpo(serialize, p);
    assert(s == "Point(3,4)");

    std::cout << "  one CPO, many tags; customize via ADL tag_invoke overloads\n";
    std::cout << "  not standardized; ranges chose per-CPO objects instead\n";
    std::cout << "tag_invoke_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section04/tag_invoke_overview", run>;

}  // namespace
