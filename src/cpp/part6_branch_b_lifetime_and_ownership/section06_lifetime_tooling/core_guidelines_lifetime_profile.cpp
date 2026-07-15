// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B9 Lifetime Profile)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : core_guidelines_lifetime_profile
// Topic id : part6/b/section06/core_guidelines_lifetime_profile
//
// 要点: Lifetime Profile 用 Owner/Pointer/View 角色静态推断悬垂。
// 与统一悬垂模型一致。
// 参考: CppCoreGuidelines Pro.lifetime

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

// Profile 角色：
// - Owner: unique_ptr / shared_ptr / 容器 / 按值 string
// - Pointer/View: T*, T&, span, string_view, iterator

struct SafeApi {
    std::string storage;
    std::string_view view() const { return storage; }  // 与 storage 同寿
};

// 反模式（概念）: Owner 返回后 View 仍被持有
// string_view leak_view() { string s="x"; return s; }

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B9 Core Guidelines Lifetime Profile ===\n";
    std::cout << "  idea: track Owner vs Pointer/View; flag Pointer past Owner\n";

    SafeApi api{"profile"};
    assert(api.view() == "profile");

    auto owner = std::make_unique<int>(3);
    [[maybe_unused]] int* raw = owner.get();  // Pointer 角色
    assert(*raw == 3);
    // Profile: Owner 释放前 Pointer 不得逃逸到更长存储

    std::vector<int> v{1, 2, 3, 4};
    std::span<int> sp = v;
    assert(sp.size() == 4);

    [[maybe_unused]] auto local_sum = [](std::span<const int> s) {
        int t = 0;
        for (int x : s) t += x;
        return t;  // 返回值，不是 view
    };
    assert(local_sum(v) == 10);

    std::cout << "  roles: Owner owns; Pointer/View borrows\n";
    std::cout << "  multilayer: types(GSL) + static(Profile/tidy) + ASan\n";
    std::cout << "core_guidelines_lifetime_profile: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/core_guidelines_lifetime_profile", run>;

}  // namespace
