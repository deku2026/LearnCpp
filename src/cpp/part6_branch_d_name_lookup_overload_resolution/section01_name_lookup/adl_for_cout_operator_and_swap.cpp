// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D2 cout/swap)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_for_cout_operator_and_swap
// Topic id : part6/d/section01/adl_for_cout_operator_and_swap
//
// 要点: operator<< 与两步 swap 都依赖 ADL。
// 参考: [basic.lookup.argdep]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace {

namespace lib {
struct Point {
    int x, y;
};
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << "," << p.y << ")";
}
int swap_hits = 0;
void swap(Point& a, Point& b) noexcept {
    ++swap_hits;
    using std::swap;
    swap(a.x, b.x);
    swap(a.y, b.y);
}
}  // namespace lib

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D2 ADL for << and swap ===\n";

    lib::Point p{1, 2};
    std::ostringstream oss;
    oss << p;  // ADL → lib::operator<<
    assert(oss.str() == "(1,2)");

    lib::Point a{1, 2}, b{3, 4};
    lib::swap_hits = 0;
    using std::swap;
    swap(a, b);  // ADL 优先 lib::swap
    assert(lib::swap_hits == 1);
    assert(a.x == 3 && a.y == 4);
    assert(b.x == 1 && b.y == 2);

    // 限定 std::swap 不走用户 swap
    lib::swap_hits = 0;
    std::swap(a, b);
    assert(lib::swap_hits == 0);
    assert(a.x == 1);

    std::cout << "adl_for_cout_operator_and_swap: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/adl_for_cout_operator_and_swap", run>;

}  // namespace
