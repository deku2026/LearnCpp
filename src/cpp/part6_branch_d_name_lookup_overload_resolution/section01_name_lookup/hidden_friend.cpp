// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D3 hidden friend)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : hidden_friend
// Topic id : part6/d/section01/hidden_friend
//
// 要点: 类内定义的友元只对 ADL 可见，普通/限定查找找不到。
// 参考: [class.friend] [basic.lookup.argdep]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace {

namespace lib {
struct Point {
    int x, y;
    friend Point operator+(const Point& a, const Point& b) { return {a.x + b.x, a.y + b.y}; }
    friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) { return os << "(" << p.x << "," << p.y << ")"; }
};
// 注意：operator+ 未在 lib 命名空间层声明
}  // namespace lib

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D3 hidden friend ===\n";

    lib::Point a{1, 2}, b{3, 4};
    lib::Point c = a + b;  // 仅 ADL
    assert(c.x == 4 && c.y == 6);
    assert((c == lib::Point{4, 6}));

    std::ostringstream oss;
    oss << c;
    assert(oss.str() == "(4,6)");

    // lib::operator+ 限定查找不可用（未注入命名空间普通成员）
    // 若取消注释通常无法编译：
    // auto d = lib::operator+(a, b);

    std::cout << "  hidden friend: ADL-only, less namespace pollution\n";
    std::cout << "hidden_friend: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/hidden_friend", run>;

}  // namespace
