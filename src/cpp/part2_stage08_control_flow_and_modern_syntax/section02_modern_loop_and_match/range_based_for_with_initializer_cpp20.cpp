// Topic    : 范围 for 带初始化器（C++20）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 2.4 / 5.5
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section02_modern_loop_and_match
// Item     : range_based_for_with_initializer_cpp20
// Topic id : part2/stage08/section02/range_based_for_with_initializer_cpp20
// Refs     : https://en.cppreference.com/w/cpp/language/range-for
//            ISO [stmt.ranged]；生命周期安全变通与 P2718 对照见 section04

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {

std::vector<int> make_data() {
    return {1, 2, 3, 4};
}

struct Inventory {
    std::map<std::string, int> stock{{"apple", 3}, {"pear", 5}};
    const std::map<std::string, int>& items() const { return stock; }
};

Inventory make_inventory() {
    return Inventory{};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [range_based_for_with_initializer_cpp20] 入门 ===\n";
    {
        // C++20：for (init-statement; for-range-declaration : range) ...
        // init 先执行，变量活过整个循环，常用来“先存住临时容器再遍历”。
        int sum = 0;
        for (auto data = make_data(); int x : data) {
            sum += x;
        }
        assert(sum == 10);
        std::cout << "for (auto data = make_data(); int x : data) sum=" << sum << '\n';
    }

    std::cout << "=== 进阶：限域计数器、锁定位、多声明 ===\n";
    {
        // 循环局部索引：不必把 i 泄漏到外层。
        std::vector<std::string> names{"A", "B", "C"};
        std::string joined;
        for (std::size_t i = 0; const auto& name : names) {
            if (i != 0) {
                joined += ',';
            }
            joined += name;
            ++i;
        }
        assert(joined == "A,B,C");
        std::cout << "index in init-statement: " << joined << '\n';

        // 先拿到 map 再结构化绑定遍历（可读 + 安全）。
        int units = 0;
        for (const auto inv = make_inventory(); const auto& [k, v] : inv.items()) {
            (void)k;
            units += v;
        }
        assert(units == 8);
        std::cout << "inventory units=" << units << '\n';

        // init 可写简单声明；C++23 起还允许 using 别名（section04）。
        int product = 1;
        for (auto data = make_data(); int x : data) {
            product *= x;
        }
        assert(product == 24);
    }

    std::cout << "=== 专家：对临时成员遍历的安全变通（C++20 时代） ===\n";
    {
        // 危险形态（C++20 及更早 UB）：for (x : make_inventory().items())
        // __range 绑的是 items() 返回的引用，临时 Inventory 在 range-init 结束即析构。
        // 完整 P2718 修复见 section04/range_for_temporary_extension_cpp23。

        // 变通 ①：具名变量
        {
            auto inv = make_inventory();
            int s = 0;
            for (const auto& [k, v] : inv.items()) {
                (void)k;
                s += v;
            }
            assert(s == 8);
        }

        // 变通 ②：C++20 范围 for 初始化器（本 topic 主题）
        {
            int s = 0;
            for (auto inv = make_inventory(); const auto& [k, v] : inv.items()) {
                (void)k;
                s += v;
            }
            assert(s == 8);
            std::cout << "C++20 init keeps temporary Inventory alive\n";
        }

        // 与 if (init; cond) 同一设计语言：把“只服务这一句控制流的临时状态”
        // 收进语句作用域，减少外层污染与悬垂窗口。
        // 选用：仅遍历返回值本身 for (x : f()) 在 C++11 起已安全（绑到临时容器）；
        //       一旦是 f().member / f()[i]，C++20 请用本初始化器或具名变量。
        std::cout << "use init-statement before f().member walks on C++20\n";
    }

    std::cout << "[range_based_for_with_initializer_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section02/range_based_for_with_initializer_cpp20", run>;

}  // namespace
