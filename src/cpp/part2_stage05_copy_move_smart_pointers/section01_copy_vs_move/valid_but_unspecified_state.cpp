// Topic    : moved-from：有效但未指定状态（valid but unspecified）
// Doc      : 第2部分-阶段5 · 步骤 1.3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : valid_but_unspecified_state
// Topic id : part2/stage05/section01/valid_but_unspecified_state
// Refs     : https://en.cppreference.com/w/cpp/utility/move
//            ISO [lib.types.movedfrom]；unique_ptr 保证为 nullptr

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

void safe_ops_on_moved_string(std::string s) {
    // 允许：无前置条件的操作
    (void)s.empty();
    (void)s.size();  // size/empty 对 string 无前置条件
    s.clear();
    s = "new value";
    assert(s == "new value");
    s = std::string{};
    // 析构总会发生
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [valid_but_unspecified_state] 入门：规则 ===\n";
    // 标准库：moved-from 对象处于 valid but unspecified state
    // · 类不变量仍成立 → 无前置条件操作安全
    // · 具体值未指定 → 不可假设「一定是空」或「一定是旧值」
    // · 例外：部分类型给出更强保证（unique_ptr → nullptr）

    std::vector<std::string> v;
    std::string str = "example";
    v.push_back(std::move(str));
    assert(v.size() == 1 && v[0] == "example");

    // str.back();  // 若 empty 则 UB —— 禁止在未检查时假设非空
    if (!str.empty()) {
        (void)str.back();  // OK：empty 无前置条件，先查再访问
    }
    str = "reassigned";
    assert(str == "reassigned");
    str.clear();
    assert(str.empty());
    std::cout << "moved-from string: only precondition-free ops\n";

    std::cout << "=== 进阶：unique_ptr 有更强保证；vector 常见为空 ===\n";
    {
        auto p = std::make_unique<int>(42);
        auto q = std::move(p);
        assert(q && *q == 42);
        assert(p.get() == nullptr);  // unique_ptr moved-from 保证为空
        assert(!p);
        std::cout << "unique_ptr after move: p is nullptr (guaranteed)\n";
    }

    {
        std::vector<int> a{1, 2, 3, 4, 5};
        std::vector<int> b = std::move(a);
        assert(b.size() == 5);
        // 标准未强制 a.empty()，但主流实现为空；只依赖合法性
        a.clear();  // 无论原先如何，clear 后确定为空
        assert(a.empty());
        a.push_back(7);  // 可重新填充
        assert(a.size() == 1 && a[0] == 7);
        std::cout << "vector after move: cleared and rebuilt\n";
    }

    std::cout << "=== 进阶：把 moved-from 交给只做安全操作的函数 ===\n";
    {
        std::string s = "temp";
        std::string t = std::move(s);
        assert(t == "temp");
        safe_ops_on_moved_string(std::move(s));  // s 已 moved-from，再 move 仍安全
        safe_ops_on_moved_string(std::move(t));
        std::cout << "safe_ops_on_moved_string completed\n";
    }

    std::cout << "=== 专家：自移动；自定义类型应保证可析构/可赋值 ===\n";
    {
        std::string self = "self";
        self = std::move(self);  // 有效但未指定；不得崩溃
        self = "ok";
        assert(self == "ok");
        std::cout << "self-move-assign then reassign OK\n";
    }

    // 自定义资源类：moved-from 至少要能析构；最好还能赋值
    // 典型做法：指针置 nullptr、size 置 0（完全确定的空状态，强于「未指定」）

    std::cout << "=== valid_but_unspecified_state: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/valid_but_unspecified_state", run>;

}  // namespace
