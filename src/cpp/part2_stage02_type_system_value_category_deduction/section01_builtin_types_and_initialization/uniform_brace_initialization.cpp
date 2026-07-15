// Topic     : 统一花括号初始化（{} 风格）
// Doc       : 第2部分-阶段2 · 步骤 2 概览（section01 侧）
// cppreference: https://en.cppreference.com/cpp/language/initialization
//               https://en.cppreference.com/cpp/language/list_initialization
//               https://en.cppreference.com/cpp/language/value_initialization
//
// 要点: {} 可用于标量/聚合/类类型；值初始化清零；免疫 most vexing parse；
//       有 initializer_list 构造时语义可能与 () 不同——不能无脑全换。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

struct Point {
    int x;
    int y;
};

struct Widget {
    int id{};
    explicit Widget() : id{1} {}
    explicit Widget(int n) : id{n} {}
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [uniform_brace_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：一种语法覆盖多种类型
    // -------------------------------------------------------------------------
    [[maybe_unused]] int n{};        // 值初始化 → 0
    [[maybe_unused]] double x{1.5};  // 直接列表初始化
    [[maybe_unused]] bool ok{true};
    std::string s{"hi"};                   // 调用匹配的构造
    [[maybe_unused]] int arr[3]{1, 2, 3};  // 聚合式
    [[maybe_unused]] Point p{10, 20};      // 聚合

    assert(n == 0 && x == 1.5 && ok);
    assert(s == "hi");
    assert(arr[0] == 1 && arr[2] == 3);
    assert(p.x == 10 && p.y == 20);
    std::cout << "[intro] scalars/arrays/aggregates/std::string with {}\n";

    // -------------------------------------------------------------------------
    // 进阶：值初始化 vs 默认初始化；explicit 与 = {}
    // -------------------------------------------------------------------------
    Widget w1{};   // 值初始化 → 默认构造
    Widget w2{7};  // 直接列表 → Widget(int)
    // Widget w3 = 7;   // ❌ explicit 构造不能 copy-init
    // Widget w4 = {7}; // ❌ explicit 通常也不能 copy-list-init（C++11+）
    assert(w1.id == 1 && w2.id == 7);

    // 嵌套与返回
    auto make_point = []() -> Point { return {3, 4}; };  // 返回聚合
    [[maybe_unused]] Point q = make_point();
    assert(q.x == 3 && q.y == 4);

    // 容器：元素列表
    std::vector<int> v{1, 2, 3};
    assert(v.size() == 3 && v[1] == 2);

    // 对比：想要“3 个值为 2 的元素”必须用圆括号（见 section02 hijack 专题）
    std::vector<int> filled(3, 2);
    assert(filled.size() == 3 && filled[0] == 2);
    std::cout << "[advanced] {} for elements; () for count+value on vector\n";

    // -------------------------------------------------------------------------
    // 专家：优先 {} 的两大理由 + 例外
    // -------------------------------------------------------------------------
    // 理由 1：窄化检查（见 narrowing_check_with_braces）
    // 理由 2：Widget w(); 是函数声明；Widget w{}; 是对象
    Widget not_a_function{};
    assert(not_a_function.id == 1);

    // 例外：拥有 initializer_list 构造的类型（vector/map/...）——{} 优先 list 构造
    std::vector<int> hijack{3, 2};  // 两个元素，不是 3 个 2
    assert(hijack.size() == 2);

    // 空花括号 T{} 在表达式中也是值初始化的 prvalue
    [[maybe_unused]] int zero = int{};
    assert(zero == 0);

    std::cout << "[expert] prefer {}; know initializer_list + MVP exceptions\n";
    std::cout << "=== uniform_brace_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section01/uniform_brace_initialization", run>;

}  // namespace
