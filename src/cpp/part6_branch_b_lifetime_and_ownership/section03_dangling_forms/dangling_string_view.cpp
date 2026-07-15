// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 string_view)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_string_view
// Topic id : part6/b/section03/dangling_string_view
//
// 要点: string_view 不拥有字符；底层 string 先死则悬垂。
// 参考: https://en.cppreference.com/w/cpp/string/basic_string_view

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// ❌ std::string_view bad() { std::string s="x"; return s; }
std::string good_owned() {
    std::string s = "hello";
    return s;
}

[[maybe_unused]] std::size_t len(std::string_view sv) {
    return sv.size();
}

struct NameView {
    std::string_view name;  // 契约：被观察 string 必须 outlive
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling string_view (safe contrasts) ===\n";

    // --- 入门 ---
    std::string owned = good_owned();
    [[maybe_unused]] std::string_view sv = owned;
    assert(sv == "hello");
    assert(len(sv) == 5);
    assert(len("literal") == 7);  // 字面量静态存储

    {
        std::string long_lived = "payload";
        [[maybe_unused]] NameView nv{long_lived};
        assert(nv.name == "payload");
    }

    // --- 进阶 ---
    std::string joined = std::string("he") + "llo";
    [[maybe_unused]] std::string_view jv = joined;
    assert(jv == "hello");

    std::string base = "abcdef";
    [[maybe_unused]] std::string_view sub = std::string_view{base}.substr(2, 3);
    assert(sub == "cde");

    // SSO 与否不影响：view 只是指针+长度，owner 销毁即悬垂
    // 函数返回 view 指向局部 —— 永远错误；返回 string

    // 容器里的 string 被 erase 后，旧 view 失效
    {
        std::vector<std::string> words{"one", "two"};
        std::string_view w0 = words[0];
        assert(w0 == "one");
        words.clear();
        // 不使用 w0
        words.emplace_back("three");
        w0 = words[0];
        assert(w0 == "three");
    }

    // --- 专家 ---
    std::cout << "  string_view = borrow; return string if you create storage\n";
    std::cout << "  member string_view requires documented lifetime contract\n";
    std::cout << "dangling_string_view: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_string_view", run>;

}  // namespace
