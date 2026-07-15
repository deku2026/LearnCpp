// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B3 string_view)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section03_dangling_forms
// Item     : dangling_string_view
// Topic id : part6/b/section03/dangling_string_view
//
// 要点: string_view 不拥有字符——底层 string 先死则悬垂。返回 string 或保证 owner 更长寿。
// 参考: https://en.cppreference.com/w/cpp/string/basic_string_view

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

// ❌ std::string_view bad() { std::string s="x"; return s; }
// ✅ 返回拥有型
std::string good_owned() {
    std::string s = "hello";
    return s;
}

// ✅ view 参数：调用方保证实参存活于调用期间
std::size_t len(std::string_view sv) {
    return sv.size();
}

// 成员存 view：文档契约——被观察 string 必须 outlive
struct NameView {
    std::string_view name;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B3 dangling string_view (safe contrasts) ===\n";

    std::string owned = good_owned();
    std::string_view sv = owned;  // 借用 owned
    assert(sv == "hello");
    assert(len(sv) == 5);
    assert(len("literal") == 7);  // 字面量静态存储，安全

    {
        std::string long_lived = "payload";
        NameView nv{long_lived};
        assert(nv.name == "payload");
        // long_lived 与 nv 同作用域结束——安全窗口
    }

    // 拼接临时：先落到 string
    std::string joined = std::string("he") + "llo";
    std::string_view jv = joined;
    assert(jv == "hello");

    // 子串 view 仍依赖 owner
    std::string base = "abcdef";
    std::string_view sub = std::string_view{base}.substr(2, 3);
    assert(sub == "cde");
    assert(base.size() == 6);

    std::cout << "  string_view = borrow; return string if you create storage\n";
    std::cout << "dangling_string_view: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section03/dangling_string_view", run>;

}  // namespace
