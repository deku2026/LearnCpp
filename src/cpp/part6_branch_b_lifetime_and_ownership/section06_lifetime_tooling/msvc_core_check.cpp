// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B9 MSVC Core Check)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : msvc_core_check
// Topic id : part6/b/section06/msvc_core_check
//
// 要点: MSVC /analyze + Core Check 报生命周期相关警告（C26800 等）。
// 展示不触发的安全路径与启用方式。
// 参考: learn.microsoft.com Code Analysis C26800 C26815 C26816

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B9 MSVC Core Check (/analyze) ===\n";
    std::cout << "  enable: cl /analyze /std:c++latest  or VS Code Analysis\n";
    std::cout << "  examples:\n";
    std::cout << "    C26800 use-after-move\n";
    std::cout << "    C26815/C26816 dangling gsl::span / string_view style\n";
    std::cout << "    C264xx lifetime / ownership family (with GSL annotations)\n";

    // 避免 use-after-move
    std::string a = "msvc";
    std::string b = std::move(a);
    assert(b == "msvc");
    a.assign("again");
    assert(a == "again");

    auto make = []() { return std::make_unique<int>(5); };
    auto p = make();
    assert(p && *p == 5);

    int x = 1;
    int* borrow = &x;
    assert(*borrow == 1);
    borrow = nullptr;

    std::cout << "  pair with ASan for runtime; Core Check is static (MSVC)\n";
    std::cout << "  B12: open Core Check / tidy and catch one issue\n";
    std::cout << "msvc_core_check: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/msvc_core_check", run>;

}  // namespace
