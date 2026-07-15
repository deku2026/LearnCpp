// Topic    : 结构化绑定（C++17）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 3
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : structured_bindings_cpp17
// Topic id : part2/stage08/section02/structured_bindings_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/structured_binding
//            提案 P0144 / P0217；ISO [dcl.struct.bind]

#include "learn/topic_registry.hpp"

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace {

struct Point {
    int x{};
    int y{};
};  // 聚合：公开数据成员，按声明顺序绑定

std::tuple<int, std::string, double> get_record() {
    return {7, "Ada", 99.5};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [structured_bindings_cpp17] 入门：解包 tuple / pair ===\n";
    {
        auto [id, name, score] = get_record();
        assert(id == 7);
        assert(name == "Ada");
        assert(score == 99.5);
        std::cout << "record: " << id << ' ' << name << ' ' << score << '\n';

        std::pair<std::string, int> kv{"port", 8080};
        auto [k, v] = kv;
        assert(k == "port" && v == 8080);
        // 旧写法：kv.first / std::get<0> / std::tie
    }

    std::cout << "=== 进阶：三种情形 + 引用限定 + 遍历 map ===\n";
    {
        // 情形① 数组
        int arr[3] = {1, 2, 3};
        auto [a, b, c] = arr;  // 拷贝数组元素到绑定（无 ref 时）
        assert(a == 1 && b == 2 && c == 3);
        a = 100;
        assert(arr[0] == 1);  // 原数组未改

        auto& [ar0, ar1, ar2] = arr;
        ar0 = 42;
        assert(arr[0] == 42);

        // 情形② tuple-like（tuple_size / tuple_element / get）
        std::array<int, 2> ar{10, 20};
        [[maybe_unused]] auto [p, q] = ar;
        assert(p == 10 && q == 20);

        // 情形③ 聚合
        Point pt{3, 4};
        [[maybe_unused]] auto [px, py] = pt;
        assert(px == 3 && py == 4);
        auto& [rx, ry] = pt;
        rx = 30;
        assert(pt.x == 30);

        // C++20：可对「部分」绑定名标 [[maybe_unused]]（实现支持时）；
        // 全绑定标 [[maybe_unused]] auto [...] 更通用。
        {
            auto tup = std::tuple{1, 2.0, std::string{"z"}};
            [[maybe_unused]] auto [only_id, ignored_d, ignored_s] = tup;
            assert(only_id == 1);
            (void)ignored_d;
            (void)ignored_s;
        }

        // ⭐ 验收点：结构化绑定 + 范围 for 遍历 map
        std::map<std::string, int> ages{{"Alice", 30}, {"Bob", 25}, {"Carol", 35}};
        [[maybe_unused]] int age_sum = 0;
        for (const auto& [person, age] : ages) {
            age_sum += age;
            std::cout << "  " << person << " is " << age << '\n';
        }
        assert(age_sum == 90);

        for (auto& [person, age] : ages) {
            (void)person;
            age += 1;
        }
        assert(ages["Alice"] == 31);
        std::cout << "map traversal + mutable binding ok\n";
    }

    std::cout << "=== 专家：隐藏对象、decltype、[[maybe_unused]]、数量匹配 ===\n";
    {
        // 结构化绑定背后有隐藏变量 e，名字是 e 的成员/get 别名（见 C++ Insights）。
        // 无 ref 时 e 是表达式结果的副本；有 & 时 e 是引用。

        auto tup = std::make_tuple(1, 2.5);
        auto [i, d] = tup;
        // decltype 给的是“被引用实体类型”，不是引用本身（即使底层是引用绑定）
        static_assert(std::is_same_v<decltype(i), int>);
        static_assert(std::is_same_v<decltype(d), double>);

        int pair_arr[2] = {5, 6};
        auto& [r0, r1] = pair_arr;
        static_assert(std::is_same_v<decltype(r0), int>);  // 仍是 int，不是 int&
        (void)r0;
        (void)r1;

        // 未使用的绑定：整句加 [[maybe_unused]]，或 (void) 点名使用。
        // 部分绑定名后标注属性依赖更新标准/扩展，教学以可移植写法为准。
        [[maybe_unused]] auto [used, unused_score] = std::tuple{42, 3.14};
        assert(used == 42);
        (void)unused_score;

        // 数量必须精确匹配，否则编译错误（此处正确匹配）。
        // 工程：insert 返回 pair 时与 if 初始化器黄金组合见
        // section02/if_switch_with_initializer_cpp17。

        // 🔶 Qt 6.4+：for (const auto& [k,v] : qmap.asKeyValueRange())
        std::cout << "tuple-like protocol: tuple_size + get<I>\n";
        std::cout << "Insights shows hidden e + get bindings\n";
    }

    std::cout << "[structured_bindings_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/structured_bindings_cpp17", run>;

}  // namespace
