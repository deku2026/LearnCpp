// Topic     : {} vs () 与 initializer_list 劫持重载决议
// Doc       : 第2部分-阶段2 · 步骤 2.3（验收点）
// cppreference: https://en.cppreference.com/cpp/language/list_initialization
//               https://en.cppreference.com/cpp/utility/initializer_list
//               https://en.cppreference.com/cpp/container/vector/vector
//
// 要点: vector(3,2) → 三个 2；vector{3,2} → 元素 3 和 2；
//       有 initializer_list 构造时 {} 优先走 list，失败不回退。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Tracker {
    std::string which;
    Tracker(std::size_t n, int v) : which{"count_value:" + std::to_string(n) + "," + std::to_string(v)} {}
    Tracker(std::initializer_list<int> il) : which{"init_list:size=" + std::to_string(il.size())} {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [braces_vs_parens_initializer_list_hijack] ===\n";

    // -------------------------------------------------------------------------
    // 入门：经典 vector 对照
    // -------------------------------------------------------------------------
    std::vector<int> v1(3, 2);  // size=3, values 2,2,2
    std::vector<int> v2{3, 2};  // size=2, values 3,2

    assert(v1.size() == 3);
    assert(v1[0] == 2 && v1[1] == 2 && v1[2] == 2);
    assert(v2.size() == 2);
    assert(v2[0] == 3 && v2[1] == 2);

    std::cout << "[intro] v1(3,2) size=" << v1.size() << " => ";
    for (int x : v1) {
        std::cout << x << ' ';
    }
    std::cout << "\n[intro] v2{3,2} size=" << v2.size() << " => ";
    for (int x : v2) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    // -------------------------------------------------------------------------
    // 进阶：自写类型复现决议
    // -------------------------------------------------------------------------
    Tracker t_parens(3, 2);
    Tracker t_braces{3, 2};
    assert(t_parens.which == "count_value:3,2");
    assert(t_braces.which == "init_list:size=2");
    std::cout << "[advanced] parens → " << t_parens.which << '\n';
    std::cout << "[advanced] braces → " << t_braces.which << '\n';

    // 单元素也要注意
    std::vector<int> one_elem{5};   // 一个元素 5
    std::vector<int> one_count(5);  // 5 个默认 0
    assert(one_elem.size() == 1 && one_elem[0] == 5);
    assert(one_count.size() == 5 && one_count[0] == 0);

    // -------------------------------------------------------------------------
    // 专家：实战建议与“不回退”
    // -------------------------------------------------------------------------
    // 想要 N 个值 / 特定构造 → 用 ()
    // 想要元素列表 → 用 {}
    // 无 initializer_list 的类型：{} 与 () 通常同义（差在窄化与 MVP）

    // 空括号 vs 空花括号
    std::vector<int> e1{};
    std::vector<int> e2;
    assert(e1.empty() && e2.empty());

    // 浮点窄化也不会让决议“改选”count 构造：{3.0, 2.0} 仍走 list（元素 double→int 窄化则整体失败）
    // std::vector<int> bad{3.0, 2.0}; // ill-formed narrowing

    std::cout << "[expert] remember: same numbers, different objects — check C++ Insights\n";
    std::cout << "=== braces_vs_parens_initializer_list_hijack: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage02/section02/braces_vs_parens_initializer_list_hijack", run>;

}  // namespace
