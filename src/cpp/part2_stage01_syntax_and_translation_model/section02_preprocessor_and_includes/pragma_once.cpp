// Topic     : #pragma once
// Doc       : 第2部分-阶段1 · 步骤 5.4
// cppreference: https://en.cppreference.com/cpp/preprocessor/include
//               https://en.cppreference.com/cpp/preprocessor/impl
//
// 要点: 非标准但主流编译器支持；按"文件身份"去重；与 include guard 对比；
//       符号链接/多路径副本边界；工程取舍。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

// 模拟"带 #pragma once 的头"。真实头文件顶部应写:
//
//   #pragma once
//   // ... declarations ...
//
// 注意: 不要在本 .cpp（主源文件）里真写 #pragma once——clang 会告
// `-Wpragma-once-outside-header`。去重语义以头文件为单位。

namespace stage01_pragma_demo {

struct Sensor {
    int channel = 0;
};

inline int sample(const Sensor& s) {
    return s.channel + 100;
}

// 双重保险写法（许多代码库两者都写）:
//   #pragma once
//   #ifndef PROJ_SENSOR_HPP
//   #define PROJ_SENSOR_HPP
//   ...
//   #endif

enum class GuardStyle { PragmaOnce, IncludeGuard, Both };

inline const char* style_name(GuardStyle s) {
    switch (s) {
        case GuardStyle::PragmaOnce:
            return "pragma_once";
        case GuardStyle::IncludeGuard:
            return "include_guard";
        case GuardStyle::Both:
            return "both";
    }
    return "unknown";
}

}  // namespace stage01_pragma_demo

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [pragma_once] ===\n";

    // -------------------------------------------------------------------------
    // §入门：一行去重
    // -------------------------------------------------------------------------
    stage01_pragma_demo::Sensor s{};
    s.channel = 7;
    assert(stage01_pragma_demo::sample(s) == 107);
    std::cout << "[intro] #pragma once marks a header as include-once for this build\n";

    // -------------------------------------------------------------------------
    // §进阶：与 include guard 对照表（运行时打印备忘）
    // -------------------------------------------------------------------------
    // | 维度         | #ifndef guard      | #pragma once        |
    // | 标准         | ISO                | 非标准（事实标准）  |
    // | 写法         | 三行+唯一宏名      | 一行                |
    // | 撞名风险     | 有                 | 无宏名              |
    // | 去重依据     | 宏是否已定义       | 实现认定的文件身份  |
    // | 多路径同一内容| 宏相同才跳过       | 可能被当成两个文件  |

    using stage01_pragma_demo::GuardStyle;
    assert(std::string{stage01_pragma_demo::style_name(GuardStyle::PragmaOnce)} == "pragma_once");
    std::cout << "[advanced] styles: " << stage01_pragma_demo::style_name(GuardStyle::PragmaOnce) << " | "
              << stage01_pragma_demo::style_name(GuardStyle::IncludeGuard) << " | "
              << stage01_pragma_demo::style_name(GuardStyle::Both) << '\n';

    // -------------------------------------------------------------------------
    // §专家：边界与建议
    // -------------------------------------------------------------------------
    // 1) GCC/Clang/MSVC 均支持 #pragma once，可移植性在主流工具链上足够好。
    // 2) 若同一头通过不同路径/挂载点被看成两个文件，once 可能失效而 guard 仍有效
    //    （宏名一致时）；反之宏撞车时 guard 更危险。
    // 3) 库对外头文件：include guard 或 both 更稳妥；应用代码: once 很常见。
    // 4) #pragma 其它用法是实现定义的；未知 pragma 通常忽略或警告。
    // 5) 验证: 建 a.hpp 仅 #pragma once + struct A{}; 在 tu.cpp 中 include 两次应通过。
    //
    // 与模块对比（预告）: C++20 modules 从模型上减少"文本粘贴头"，但大量代码仍依赖
    // include 世界，guard/once 仍是日常技能。

    const GuardStyle recommended_for_libs = GuardStyle::IncludeGuard;
    const GuardStyle app_ok = GuardStyle::PragmaOnce;
    assert(recommended_for_libs != app_ok || true);
    std::cout << "[expert] libraries: prefer portable guards; apps often use #pragma once\n";
    std::cout << "=== pragma_once: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/pragma_once", run>;

}  // namespace
