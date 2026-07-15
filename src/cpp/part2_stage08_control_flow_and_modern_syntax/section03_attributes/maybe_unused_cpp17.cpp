// Topic    : [[maybe_unused]]（C++17）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 6.3
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : maybe_unused_cpp17
// Topic id : part2/stage08/section03/maybe_unused_cpp17
// Refs     : https://en.cppreference.com/w/cpp/language/attributes/maybe_unused
//            ISO [dcl.attr.unused]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 参数在某些配置下不用：标 maybe_unused，避免 -Wunused-parameter。
[[maybe_unused]] int scale(int value, [[maybe_unused]] int debug_level) {
#ifdef LEARN_TRACE
    std::cerr << "scale debug=" << debug_level << '\n';
#endif
    return value * 2;
}

// 枚举常量可能只用于静态检查/文档。
enum class Color { Red, Green, Blue };

[[maybe_unused]] constexpr Color kDefaultAccent = Color::Green;

template <typename T>
int size_tag() {
    // 编译期分支：假分支被丢弃，真分支才实例化
    if constexpr (std::is_integral_v<T>) {
        return static_cast<int>(sizeof(T));
    } else {
        return -1;
    }
}

// 枚举项 / 全局常量：声明了但当前翻译单元未必 ODR 使用
enum class TraceChannel { Off, Verbose };
[[maybe_unused]] constexpr TraceChannel kTrace = TraceChannel::Off;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [maybe_unused_cpp17] 入门：变量与参数 ===\n";
    {
        assert(scale(21, 3) == 42);

        // 只为可读性/未来扩展保留的结果。
        [[maybe_unused]] int future_metric = 0;
        [[maybe_unused]] int used = 1;
        assert(used == 1);
        std::cout << "scale(21)=42; unused locals silenced\n";
    }

    std::cout << "=== 进阶：结构化绑定、断言、宏配置 ===\n";
    {
        // 整句绑定可标 maybe_unused；按名标注是更新的扩展（工具链支持不一）。
        auto tup = std::make_pair(10, std::string{"x"});
        [[maybe_unused]] auto [id, name] = tup;
        assert(id == 10);
        (void)name;

        // assert 在 NDEBUG 下会吞掉表达式：其中用到的变量在 Release 变“未使用”。
        int checksum = 5 + 5;
        assert(checksum == 10);
        [[maybe_unused]] int only_for_assert = checksum;
        // 若某变量只出现在 assert(...) 内，应标 maybe_unused，或在 assert 外再 ODR 使用。

        std::cout << "pair id=" << id << " (name maybe_unused)\n";
        assert(size_tag<int>() == static_cast<int>(sizeof(int)));
        assert(size_tag<std::string>() == -1);
        assert(kTrace == TraceChannel::Off);
    }

    std::cout << "=== 专家：可标注实体、与 void 转型、过度使用 ===\n";
    {
        // 可标注：类、typedef、变量、非静态数据成员、函数、枚举、枚举项。
        // 目的：抑制“未使用”诊断，不改变语义与目标代码。

        // 对比 (void)x：语句级丢弃，适合单点；
        // maybe_unused：声明级，适合整段配置差异。

        [[maybe_unused]] auto quiet = [](int x) {
            (void)x;  // 另一种压警告方式
            return 0;
        };
        assert(quiet(1) == 0);

        // 过度使用会掩盖真的死代码；优先删除无用实体，属性留给“条件性使用”。
        // 与 nodiscard：二者正交——一个管返回值必须用，一个管实体允许不用。
        std::cout << "use for config-dependent names; delete truly dead code\n";
        std::cout << "kDefaultAccent is a maybe_unused enum constant example\n";
        assert(static_cast<int>(kDefaultAccent) == static_cast<int>(Color::Green));
    }

    std::cout << "[maybe_unused_cpp17] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/maybe_unused_cpp17", run>;

}  // namespace
