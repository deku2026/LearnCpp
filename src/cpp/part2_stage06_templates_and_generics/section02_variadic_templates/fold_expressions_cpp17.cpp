// Topic    : 折叠表达式 —— 一元/二元左右折叠与空包规则
// Doc      : 第2部分-阶段6 · 步骤 7.3
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : fold_expressions_cpp17
// Topic id : part2/stage06/section02/fold_expressions_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/fold
//            ISO [expr.prim.fold]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

// ---------------------------------------------------------------------------
// §入门 — 一元右/左折叠
// ---------------------------------------------------------------------------

template <typename... Args>
auto sum_right(Args... args) {
    return (args + ...);  // 一元右折叠
}

template <typename... Args>
bool all_true(Args... args) {
    return (... && args);  // 一元左折叠；空包 → true
}

template <typename... Args>
bool any_true(Args... args) {
    return (... || args);  // 空包 → false
}

// ---------------------------------------------------------------------------
// §进阶 — 二元折叠（带初值，空包安全）与逗号折叠
// ---------------------------------------------------------------------------

template <typename... Args>
auto sum_init(Args... args) {
    return (0 + ... + args);  // 二元左折叠；空包 → 0
}

template <typename... Args>
void print_line(Args... args) {
    ((std::cout << args << ' '), ...);  // 逗号折叠
    std::cout << '\n';
}

template <typename... Args>
std::string join_csv(Args... args) {
    std::ostringstream os;
    bool first = true;
    auto sep = [&](const auto& x) {
        if (!first) {
            os << ',';
        }
        first = false;
        os << x;
    };
    (sep(args), ...);
    return os.str();
}

template <typename C, typename... Args>
void push_all(C& c, Args&&... args) {
    (c.push_back(std::forward<Args>(args)), ...);
}

// ---------------------------------------------------------------------------
// §专家 — 空包规则；折叠与类型混合；赋值折叠
// ---------------------------------------------------------------------------

template <typename... Args>
auto product_or_one(Args... args) {
    // 一元 * 遇空包非法 → 必须二元带 1
    return (1 * ... * args);
}

template <typename T, typename... Args>
void assign_all(T& dst, Args... /*ignored*/) {
    // 演示：折叠赋值链需要同一对象时用逗号
    ((void)Args{}, ...);
    dst = T{};
}

template <typename... Args>
int count_positive(Args... args) {
    return (0 + ... + (args > 0 ? 1 : 0));
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [fold_expressions_cpp17] 入门 ===\n";
    assert(sum_right(1, 2, 3, 4) == 10);
    assert(all_true(true, true, true));
    assert(!all_true(true, false, true));
    assert(all_true());  // 空包 && → true
    assert(!any_true());
    assert(any_true(false, true, false));
    std::cout << "sum_right=" << sum_right(1, 2, 3, 4) << '\n';

    std::cout << "=== 进阶：初值折叠 / 逗号折叠 / push_all ===\n";
    assert(sum_init() == 0);
    assert(sum_init(5, 5) == 10);
    print_line(1, 2.5, "three", 'x');
    assert(join_csv(1, 2, 3) == "1,2,3");
    assert(join_csv() == "");
    std::vector<int> v;
    push_all(v, 10, 20, 30);
    assert((v == std::vector<int>{10, 20, 30}));
    std::cout << "join_csv=" << join_csv(1, 2, 3) << '\n';

    std::cout << "=== 专家：空包与条件折叠 ===\n";
    assert(product_or_one() == 1);
    assert(product_or_one(2, 3, 4) == 24);
    assert(count_positive(-1, 0, 2, 5, -3) == 2);
    // ⚠️ (args + ...) 在空包时编译错误；只有 && || , 的一元折叠允许空包
    int x = 99;
    assign_all(x, 1, 2);
    assert(x == 0);
    std::cout << "=== fold_expressions_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/fold_expressions_cpp17", run>;

}  // namespace
