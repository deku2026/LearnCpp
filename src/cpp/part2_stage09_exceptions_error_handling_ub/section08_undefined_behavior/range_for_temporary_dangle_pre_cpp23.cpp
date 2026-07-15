// Topic    : range-for 临时悬垂（C++23 前）与 C++23 修复对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.2
//            阶段 8 range-for 临时扩展
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : range_for_temporary_dangle_pre_cpp23
// Topic id : part2/stage09/section08/range_for_temporary_dangle_pre_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/range-for
//            P2718 (C++23 lifetime extension in range-for)

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

std::vector<std::string> make_names() {
    return {"Ann", "Bob", "Cara"};
}

// 返回临时 vector 的“视图式”包装（教学用）
struct NamesView {
    std::vector<std::string> data;
    const std::vector<std::string>& items() const { return data; }
};

NamesView make_view() {
    return NamesView{std::vector<std::string>{"x", "y"}};
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [range_for_temporary_dangle] 主干：C++23 安全写法 ===\n";
    {
        // 直接 range-for 绑定工厂返回的临时：C++23 延长整个基于范围的 for 期间
        // 临时对象寿命（P2718）。本工程按 C++23 编译，路径良定义。
        int n = 0;
        for (const auto& s : make_names()) {
            (void)s;
            assert(!s.empty());
            ++n;
        }
        assert(n == 3);
        std::cout << "for (auto& s : make_names()) count=" << n << " (C++23 OK)\n";
    }

    std::cout << "=== 对抗：显式具名，所有标准都安全 ===\n";
    {
        const auto names = make_names();  // 具名，寿命清晰
        std::string concat;
        for (const auto& s : names) {
            concat += s;
        }
        assert(concat == "AnnBobCara");
        std::cout << "named temporary: " << concat << '\n';
    }

    std::cout << "=== 专节：C++23 前的经典坑（注释，不触发 UB）===\n";
    // C++20 及更早的危险模式（示意）：
    //   for (auto& x : make_view().items()) { use(x); }
    // 问题：range-for 只延长“冒号右侧那个表达式”的临时；
    // make_view() 临时在拿到 items() 引用后可能已销毁 → 遍历悬垂。
    // C++23 P2718：扩大寿命扩展，覆盖这类链式临时。
    // 可移植纪律：先 autov = factory(); for (auto& x : v) ...
    {
        // 安全改写链式访问
        const auto view = make_view();
        int n = 0;
        for (const auto& s : view.items()) {
            (void)s;
            ++n;
            assert(s.size() == 1);
        }
        assert(n == 2);
        std::cout << "named view.items() always safe, n=" << n << '\n';
    }

    std::cout << "prefer named locals when targeting pre-C++23\n";
    std::cout << "[range_for_temporary_dangle_pre_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/range_for_temporary_dangle_pre_cpp23", run>;

}  // namespace
