// LearnCpp topic
// Doc      : part6-branch-d-name-lookup-overload-resolution.md (D1 using)
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : using_introduction
// Topic id : part6/d/section01/using_introduction
//
// 要点: using ns::name / using Base::f / using enum 把名字引入当前作用域。
// 参考: [namespace.udecl]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

namespace tools {
[[maybe_unused]] int answer() {
    return 42;
}
[[maybe_unused]] std::string name() {
    return "tools";
}
}  // namespace tools

struct Base {
    int g(int x) { return x + 1; }
};

struct D : Base {
    using Base::g;
    int g(double x) { return static_cast<int>(x) + 10; }
};

enum class Color { Red, Green, Blue };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== D1 using introduction ===\n";

    using tools::answer;
    assert(answer() == 42);
    assert(tools::name() == "tools");

    [[maybe_unused]] D d;
    assert(d.g(1) == 2);
    assert(d.g(1.0) == 11);

    using enum Color;
    [[maybe_unused]] Color c = Green;
    assert(c == Color::Green);

    using std::swap;
    int a = 1, b = 2;
    swap(a, b);
    assert(a == 2 && b == 1);

    std::cout << "using_introduction: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/d/section01/using_introduction", run>;

}  // namespace
