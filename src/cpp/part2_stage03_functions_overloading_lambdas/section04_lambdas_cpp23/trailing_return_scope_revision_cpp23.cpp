// Topic     : C++23 lambda 尾置返回与作用域修订
// Doc       : 第2部分-阶段3 · 步骤 6 / lambda 演进
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 尾置返回类型中的名字查找以参数列表之后为准；与参数同名/依赖参数类型更清晰；
//       结合显式模板参数与 trailing return；对照普通函数尾置返回。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [trailing_return_scope_revision_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：lambda 尾置返回
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto scale = [](int x) -> double { return x * 0.5; };
    assert(scale(10) == 5.0);
    std::cout << "[intro] [](int x) -> double { ... }\n";

    // -------------------------------------------------------------------------
    // §进阶：返回类型依赖参数
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto add = [](const auto& a, const auto& b) -> decltype(a + b) { return a + b; };
    assert(add(1, 2) == 3);
    assert(add(1.5, 2.5) == 4.0);

    [[maybe_unused]] auto first = []<class T>(const std::vector<T>& v) -> T { return v.empty() ? T{} : v.front(); };
    std::vector<int> v{9, 8, 7};
    assert(first(v) == 9);
    std::cout << "[advanced] trailing decltype / template lambda return\n";

    // -------------------------------------------------------------------------
    // §专家：作用域与可读性修订
    // -------------------------------------------------------------------------
    // 尾置返回写在参数之后，可用参数名参与 decltype（与函数尾置返回同一动机）。
    // C++23 语法放宽（省略 ()、属性、static）不改变「-> 类型」的写法，
    // 但让「无参 + 属性 + 尾置返回」更整齐：
    [[maybe_unused]] auto unit = [] [[nodiscard]] () -> int { return 1; };
    assert(unit() == 1);

    // 完美转发风格
    auto fwd = []<class T>(T&& x) -> T&& { return std::forward<T>(x); };
    int n = 3;
    static_assert(std::is_same_v<decltype(fwd(n)), int&>);
    assert(fwd(n) == 3);

    // 多返回路径时显式 -> T 仍是消除推导分歧的首选
    [[maybe_unused]] auto sign = [](int x) -> int {
        if (x > 0) {
            return 1;
        }
        if (x < 0) {
            return -1;
        }
        return 0;
    };
    assert(sign(-5) == -1);

    std::cout << "[expert] trailing return keeps param names in scope for decltype\n";
    std::cout << "=== trailing_return_scope_revision_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/trailing_return_scope_revision_cpp23", run>;

}  // namespace
