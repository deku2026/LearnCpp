// Topic     : std::initializer_list 参数（任意个同类型实参）
// Doc       : 第2部分-阶段3 · 步骤 8.1
// cppreference: https://en.cppreference.com/cpp/utility/initializer_list
//
// 要点: 花括号传入任意个同类型（或可隐式转换）元素；轻量视图，不拥有堆上副本的所有权语义要小心；
//       元素类型单一——混合类型请用可变参数模板（阶段 6）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace {

[[maybe_unused]] int sum(std::initializer_list<int> nums) {
    int total = 0;
    for (int n : nums) {
        total += n;
    }
    return total;
}

[[maybe_unused]] double average(std::initializer_list<double> xs) {
    if (xs.size() == 0) {
        return 0.0;
    }
    double s = 0.0;
    for (double x : xs) {
        s += x;
    }
    return s / static_cast<double>(xs.size());
}

[[maybe_unused]] std::string join(std::initializer_list<std::string> parts, char sep = ',') {
    std::string out;
    bool first = true;
    for (const auto& p : parts) {
        if (!first) {
            out += sep;
        }
        out += p;
        first = false;
    }
    return out;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [initializer_list_parameter] ===\n";

    // -------------------------------------------------------------------------
    // §入门：花括号传任意个同类型
    // -------------------------------------------------------------------------
    assert(sum({1, 2, 3}) == 6);
    assert(sum({10, 20, 30, 40}) == 100);
    assert(sum({}) == 0);
    std::cout << "[intro] sum({...}) accepts any count of int\n";

    // -------------------------------------------------------------------------
    // §进阶：迭代、size、隐式转换
    // -------------------------------------------------------------------------
    assert(average({1.0, 2.0, 3.0}) == 2.0);
    // int 可隐式转 double
    assert(average({2, 4, 6}) == 4.0);
    assert(join({"a", "b", "c"}) == "a,b,c");
    assert(join({"x"}, ';') == "x");

    // 构造函数场景（回顾阶段 2）：vector 从 initializer_list 构造
    std::vector<int> v{1, 2, 3, 4};
    assert(v.size() == 4);
    std::cout << "[advanced] size/iteration; element conversions; vector ctor\n";

    // -------------------------------------------------------------------------
    // §专家：视图语义与边界
    // -------------------------------------------------------------------------
    // initializer_list 内部是指针 + 长度，指向「临时数组」；
    // ⚠️ 不要把 initializer_list 本身存到超出完整表达式生命周期之后再解引用。
    // 若要长期持有，拷贝进 vector/string 等拥有型容器。
    [[maybe_unused]] std::initializer_list<int> il = {7, 8, 9};  // 教学演示：完整表达式内使用 OK
    assert(std::accumulate(il.begin(), il.end(), 0) == 24);

    // 限制：不能直接混异构类型 print({1, "x"}) 成同一 initializer_list。
    // 需要异构包 → template<class... Args> / 折叠表达式（见 variadic_templates_intro）。

    std::cout << "[expert] initializer_list is a view into a temporary array; copy to own\n";
    std::cout << "=== initializer_list_parameter: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/initializer_list_parameter", run>;

}  // namespace
