// Topic     : std::bind vs lambda：优先 lambda
// Doc       : 第2部分-阶段3 · 步骤 7.6
// cppreference: https://en.cppreference.com/cpp/utility/functional/bind
//               https://en.cppreference.com/cpp/utility/functional/bind_front
//
// 要点: bind + 占位符晦涩、易错、优化不友好；现代代码几乎总用 lambda；
//       简单绑前/绑后用 bind_front/bind_back。

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <version>

#if !defined(__cpp_lib_bind_back) || !(__cpp_lib_bind_back)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::bind_back not available (__cpp_lib_bind_back)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_versus_lambda", run>;
}  // namespace
#else
namespace {

int sub(int a, int b) {
    return a - b;
}

int mul_add(int a, int b, int c) {
    return a * b + c;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_bind_versus_lambda] ===\n";

    // -------------------------------------------------------------------------
    // §入门：同一意图的三种写法
    // -------------------------------------------------------------------------
    using namespace std::placeholders;

    // 老：std::bind + 占位符
    [[maybe_unused]] auto f_bind = std::bind(sub, _1, 3);  // f(a) == sub(a, 3)
    // 新：bind_back
    [[maybe_unused]] auto f_back = std::bind_back(sub, 3);
    // 推荐：lambda
    [[maybe_unused]] auto f_lam = [](int a) { return sub(a, 3); };

    assert(f_bind(10) == 7);
    assert(f_back(10) == 7);
    assert(f_lam(10) == 7);
    std::cout << "[intro] bind / bind_back / lambda same result; lambda is clearest\n";

    // -------------------------------------------------------------------------
    // §进阶：重排参数时 lambda 仍更清晰
    // -------------------------------------------------------------------------
    // bind 重排: mul_add(c, a, b) 之类
    [[maybe_unused]] auto reordered_bind = std::bind(mul_add, _3, _1, _2);  // (a,b,c) -> mul_add(c,a,b)
    [[maybe_unused]] auto reordered_lam = [](int a, int b, int c) { return mul_add(c, a, b); };
    assert(reordered_bind(2, 3, 4) == mul_add(4, 2, 3));
    assert(reordered_lam(2, 3, 4) == 4 * 2 + 3);
    std::cout << "[advanced] argument reordering is readable as a lambda body\n";

    // -------------------------------------------------------------------------
    // §专家：bind 的坑与现代替代矩阵
    // -------------------------------------------------------------------------
    // 1) bind 默认拷贝实参；要引用需 std::ref —— 易忘导致「改了没反应」。
    int n = 1;
    [[maybe_unused]] auto add_n_bind = std::bind(std::plus<>{}, std::ref(n), _1);
    [[maybe_unused]] auto add_n_lam = [&n](int x) { return n + x; };
    n = 10;
    assert(add_n_bind(5) == 15);
    assert(add_n_lam(5) == 15);

    // 2) 嵌套 bind 有特殊「解绑」规则，难读难维护。
    // 3) 替代矩阵：
    //    - 简单前/后缀绑定 → bind_front / bind_back
    //    - 其它一切 → lambda
    //    - 类型擦除存储 → function / move_only_function
    // 4) Effective Modern C++ 条款 34：优先 lambda 而非 std::bind。
    std::string prefix = "X-";
    auto tag = [prefix](const std::string& s) { return prefix + s; };
    assert(tag("id") == "X-id");

    std::cout << "[expert] prefer lambda; bind_front/back for trivial partial apply only\n";
    std::cout << "=== std_bind_versus_lambda: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_versus_lambda", run>;

}  // namespace
#endif  // __cpp_lib_bind_back
