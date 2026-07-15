// Topic    : range-for 临时对象生命周期延长（P2718, C++23）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 5
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section04_cpp23_statement_and_preprocess
// Item     : range_for_temporary_extension_cpp23
// Topic id : part2/stage08/section04/range_for_temporary_extension_cpp23
// Refs     : https://en.cppreference.com/w/cpp/language/range-for
//            提案 P2718R0；ISO [stmt.ranged] [class.temporary]
// 重要安全修复：for (x : f().member) 在 C++23 下保活 f() 临时

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// 用析构日志观察：循环结束前临时 Widget 是否还活着。
struct Widget {
    std::vector<int> items_{10, 20, 30};
    std::string tag_;
    inline static int live = 0;
    [[maybe_unused]] inline static int destroyed_during_loop = 0;

    explicit Widget(std::string tag) : tag_(std::move(tag)) { ++live; }

    Widget(const Widget& o) : items_(o.items_), tag_(o.tag_ + "/copy") { ++live; }

    Widget(Widget&& o) noexcept : items_(std::move(o.items_)), tag_(std::move(o.tag_)) {
        ++live;
        o.tag_ = "moved-from";
    }

    ~Widget() {
        --live;
        // 若循环仍在进行而 live 已减，说明临时过早析构（C++20 风险模型）
        tag_.clear();
    }

    const std::vector<int>& items() const { return items_; }
};

Widget make_widget() {
    return Widget{"temp"};
}

int sum_items_cpp23_style() {
    int sum = 0;
    // C++23 P2718：range-initializer 中创建的临时 Widget 延长到整个循环结束。
    // 即使 __range 绑的是 items() 的引用，父临时也被保活。
    for (int x : make_widget().items()) {
        sum += x;
        assert(Widget::live >= 1 && "temporary should still be alive in C++23");
    }
    return sum;
}

[[maybe_unused]] int sum_items_named() {
    int sum = 0;
    auto w = make_widget();  // C++20 变通①：具名变量
    for (int x : w.items()) {
        sum += x;
    }
    return sum;
}

[[maybe_unused]] int sum_items_range_init() {
    int sum = 0;
    // C++20 变通②：范围 for 初始化器
    for (auto w = make_widget(); int x : w.items()) {
        sum += x;
    }
    return sum;
}

[[maybe_unused]] int sum_direct_temporary_vector() {
    int sum = 0;
    // 直接绑到临时 vector：C++11 起就安全（__range 直接延长该临时）
    for (int x : std::vector<int>{1, 2, 3}) {
        sum += x;
    }
    return sum;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [range_for_temporary_extension_cpp23] 入门：P2718 安全写法 ===\n";
    {
        const int s = sum_items_cpp23_style();
        assert(s == 60);
        std::cout << "for (x : make_widget().items()) sum=" << s << " (C++23)\n";
        assert(Widget::live == 0);
    }

    std::cout << "=== 进阶：安全/危险形态判别 + C++20 变通 ===\n";
    {
        // 口诀：
        // for (x : f())           — 直接绑临时 → 一直安全
        // for (x : f().member)    — C++20 悬垂风险；C++23 P2718 修复
        assert(sum_direct_temporary_vector() == 6);
        assert(sum_items_named() == 60);
        assert(sum_items_range_init() == 60);
        std::cout << "named temp & range-for init work on C++20 too\n";

        // 概念展开（C++23）：
        // auto&& __temp  = make_widget();
        // auto&& __range = __temp.items();
        // for (begin..end) use *__begin;
    }

    std::cout << "=== 专家：编译器实现落差、ASan 验收、工程纪律 ===\n";
    {
        // 现实：P2718 落地偏晚（文档记录 GCC 15+ 等）；“-std=c++23”≠必有 P2718。
        // 目标工具链未实现前，必须坚持具名变量 / 范围 for 初始化器。
        //
        // 验收建议（勿在本可运行 topic 里故意造 UB）：
        // - 同一代码分别以 C++20 + ASan 与 C++23 编译
        // - C++20 可能 stack-use-after-scope；C++23（已实现时）干净
        //
        // 常见危险 API 形态：getConfig().values、parse().items、f()[i]
        // 测试环境“碰巧通过”、生产高负载才崩 —— 极难查。

        [[maybe_unused]] int again = 0;
        for (int x : make_widget().items()) {
            again += x;
        }
        assert(again == 60);
        assert(Widget::live == 0);

        std::cout << "verify P2718 on target compiler before relying on it\n";
        std::cout << "until then: auto tmp = f(); for (x : tmp.member)\n";
    }

    std::cout << "[range_for_temporary_extension_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section04/range_for_temporary_extension_cpp23", run>;

}  // namespace
