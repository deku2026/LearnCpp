// Topic     : lambda 引用捕获与悬垂风险
// Doc       : 第2部分-阶段3 · 步骤 5.3 / 5.5
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [&x] / [&] 不延长生命周期；当场用完（sort 比较器）安全高效；
//       存起来 / 异步 / 返回后调用 → 悬垂 UB；修法：值捕获或延长被引用对象寿命。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

namespace {

// ⚠️ 仅作「错误模式」文字对照——真正调用悬垂是 UB，本文件不执行
// std::function<int()> make_bad() {
//     int local = 42;
//     return [&local] { return local; };
// }

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [capture_by_reference] ===\n";

    // -------------------------------------------------------------------------
    // §入门：[&x] 看到外部最新值，并可修改
    // -------------------------------------------------------------------------
    int n = 0;
    auto bump = [&n] { ++n; };
    bump();
    bump();
    assert(n == 2);

    int a = 1;
    int b = 2;
    auto sum_ref = [&] { return a + b; };
    a = 10;
    b = 20;
    assert(sum_ref() == 30);
    std::cout << "[intro] ref capture aliases outer variables\n";

    // -------------------------------------------------------------------------
    // §进阶：算法中的「当场用完」
    // -------------------------------------------------------------------------
    std::vector<int> v{3, 1, 4, 1, 5};
    int threshold = 2;
    // 比较/谓词在 sort/count 返回前就用完 → [&] 安全
    const auto cnt = std::count_if(v.begin(), v.end(), [&](int x) { return x > threshold; });
    assert(cnt == 3);

    std::sort(v.begin(), v.end(), [&](int lhs, int rhs) {
        // 可用外部策略变量
        return lhs < rhs;
    });
    assert(v.front() == 1 && v.back() == 5);
    std::cout << "[advanced] [&] OK when lambda does not outlive locals (algorithms)\n";

    // -------------------------------------------------------------------------
    // §专家：悬垂边界与修法
    // -------------------------------------------------------------------------
    // 闭包不延长被引用实体的寿命。返回/存入成员/丢进线程后若局部已销毁 → UB。
    // 修法：
    //   1) 值捕获 [local]
    //   2) init-capture [local = local] / move
    //   3) 保证被引用对象寿命盖过所有调用（例如捕获成员时用 *this 或共享所有权）
    int keep = 7;
    std::function<int()> safe = [keep] { return keep; };
    assert(safe() == 7);

    // 混合：默认引用，个别按值
    int secret = 99;
    auto mix = [&, secret] { return secret + n; };
    secret = 0;
    assert(mix() == 99 + n);  // secret 仍是捕获时的 99

    std::cout << "[expert] dangling ref capture is UB; prefer value when storing callables\n";
    std::cout << "=== capture_by_reference: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_by_reference", run>;

}  // namespace
