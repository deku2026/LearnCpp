// Topic     : 列表初始化 list initialization
// Doc       : 第2部分-阶段2 · 步骤 2.1–2.3
// cppreference: https://en.cppreference.com/cpp/language/list_initialization
//               https://en.cppreference.com/cpp/utility/initializer_list
//
// 要点: T x{a,b} / T x = {a,b} / T({a,b}) / return {..}；
//       禁止窄化；有 initializer_list 构造时优先选之。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Bag {
    std::vector<int> data;
    Bag(std::initializer_list<int> il) : data{il} {}
    Bag(int n, int v) : data(static_cast<std::size_t>(n), v) {}
};

struct Agg {
    int x;
    int y;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [list_initialization] ===\n";

    // -------------------------------------------------------------------------
    // 入门：直接列表 vs 拷贝列表
    // -------------------------------------------------------------------------
    [[maybe_unused]] int a{1};
    [[maybe_unused]] int b = {2};
    [[maybe_unused]] int arr[]{1, 2, 3};
    [[maybe_unused]] Agg p{4, 5};
    assert(a == 1 && b == 2 && arr[2] == 3 && p.x == 4 && p.y == 5);

    // 禁止窄化
    // int bad{1.5};
    [[maybe_unused]] int ok{static_cast<int>(1.5)};
    assert(ok == 1);
    std::cout << "[intro] brace init for scalars/arrays/aggregates; no narrowing\n";

    // -------------------------------------------------------------------------
    // 进阶：initializer_list 优先
    // -------------------------------------------------------------------------
    Bag b1{3, 2};  // initializer_list → data == {3,2}
    assert(b1.data.size() == 2 && b1.data[0] == 3 && b1.data[1] == 2);

    Bag b2(3, 2);  // 圆括号 → (count,value) → {2,2,2}
    assert(b2.data.size() == 3 && b2.data[0] == 2);

    std::vector<std::string> words{"alpha", "beta"};
    assert(words.size() == 2 && words[0] == "alpha");

    // 空列表
    std::vector<int> empty{};
    assert(empty.empty());
    std::cout << "[advanced] list-init prefers initializer_list ctor when viable\n";

    // -------------------------------------------------------------------------
    // 专家：两阶段决议（cppreference list-initialization）
    // -------------------------------------------------------------------------
    // 1) 若存在 initializer_list 构造且元素可转换（非窄化）→ 只在这些里决议
    // 2) 否则全部构造函数参与，且转换不得窄化
    // 失败不会“回退”到 (size,value) 等其它构造——这就是“劫持”的硬边

    // 函数返回 / 临时量
    auto make_agg = []() -> Agg { return {9, 10}; };
    [[maybe_unused]] Agg q = make_agg();
    assert(q.x == 9 && q.y == 10);

    // 嵌套列表
    std::vector<std::pair<int, int>> pairs{{1, 2}, {3, 4}};
    assert(pairs.size() == 2 && pairs[1].second == 4);

    std::cout << "[expert] list-init is its own initialization kind with 2-phase rules\n";
    std::cout << "=== list_initialization: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/list_initialization", run>;

}  // namespace
