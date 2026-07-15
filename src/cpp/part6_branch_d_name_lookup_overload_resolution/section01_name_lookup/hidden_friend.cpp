// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D3 hidden friend)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : hidden_friend
// Topic id : part6/d/section01/hidden_friend
//
// 要点: 类内定义的友元只被 ADL 找到；普通/限定查找找不到。
// 参考: [class.friend] [basic.lookup.argdep]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <sstream>
#include <string>

namespace {

namespace lib {
struct Point {
    int x, y;
    friend Point operator+(const Point& a, const Point& b) { return {a.x + b.x, a.y + b.y}; }
    [[maybe_unused]] friend bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) { return os << "(" << p.x << "," << p.y << ")"; }
    friend int magnitude2(const Point& p) { return p.x * p.x + p.y * p.y; }
};
// operator+ 未注入 lib 命名空间普通成员表
}  // namespace lib

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D3 hidden friend ===\n";

    lib::Point a{1, 2}, b{3, 4};
    lib::Point c = a + b;  // 仅 ADL
    assert(c.x == 4 && c.y == 6);
    assert((c == lib::Point{4, 6}));
    assert(magnitude2(c) == 16 + 36);

    std::ostringstream oss;
    oss << c;
    assert(oss.str() == "(4,6)");

    // 限定 lib::operator+ 通常找不到（未作为命名空间成员声明）
    // auto d = lib::operator+(a, b); // ill-formed（典型实现）

    std::cout << "  hidden friend: ADL-only, less namespace pollution\n";
    std::cout << "  ideal for operators associated with one class\n";
    std::cout << "hidden_friend: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/hidden_friend", run>;

}  // namespace
