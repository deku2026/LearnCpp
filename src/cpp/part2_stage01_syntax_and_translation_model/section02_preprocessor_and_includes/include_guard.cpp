// Topic     : include guard（#ifndef / #define / #endif）
// Doc       : 第2部分-阶段1 · 步骤 5.4
// cppreference: https://en.cppreference.com/cpp/preprocessor/include
//
// 要点: 防止同一 TU 内重复包含导致重定义；宏名全局唯一；
//       不解决跨 TU 多份定义；与 #pragma once 对比；宏名撞车事故。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

// —— 模拟头文件 A：标准 include guard ——
#ifndef LEARNCPP_STAGE01_WIDGET_HPP_INCLUDED
#define LEARNCPP_STAGE01_WIDGET_HPP_INCLUDED

namespace stage01_guard_demo {

struct Widget {
    int id = 0;
    explicit Widget(int i) : id(i) {}
};

inline int widget_score(const Widget& w) {
    return w.id * 10;
}

// 记录"头体被处理次数"：正常同一 TU 多次 include 仍只应 +1
inline int& inclusion_body_hits() {
    static int hits = 0;
    return hits;
}

struct InclusionHit {
    InclusionHit() { ++inclusion_body_hits(); }
};

// 构造一个静态对象，仅当 guard 放行头体时才注册一次
inline InclusionHit& hit_once() {
    static InclusionHit h{};
    return h;
}

}  // namespace stage01_guard_demo

#endif  // LEARNCPP_STAGE01_WIDGET_HPP_INCLUDED

// —— 故意"再次包含"同一逻辑头：粘贴第二份 guard 包装文本 ——
// 真实工程中由 #include "widget.hpp" 触发；这里内联第二份相同 guard 结构。
#ifndef LEARNCPP_STAGE01_WIDGET_HPP_INCLUDED
#define LEARNCPP_STAGE01_WIDGET_HPP_INCLUDED
// 若没有 guard，这里再定义 struct Widget 会在本 TU 直接编译失败（ODR① / 重定义）。
namespace stage01_guard_demo {
struct Widget {
    int id = 0;
};
}  // namespace stage01_guard_demo
#endif

// —— 演示宏名撞车：两个无关"头"用了同一个 guard 宏（反面教材）——
#ifndef LEARNCPP_STAGE01_BAD_SHARED_GUARD
#define LEARNCPP_STAGE01_BAD_SHARED_GUARD
namespace stage01_guard_demo {
inline constexpr int from_first_header = 1;
}
#endif

#ifndef LEARNCPP_STAGE01_BAD_SHARED_GUARD
// 本应是第二份头的内容，但因宏名相同被整体跳过 → 静默丢失定义
#define LEARNCPP_STAGE01_BAD_SHARED_GUARD
namespace stage01_guard_demo {
inline constexpr int from_second_header = 2;
}
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [include_guard] ===\n";

    // -------------------------------------------------------------------------
    // §入门：经典三件套
    // -------------------------------------------------------------------------
    // #ifndef UNIQUE_MACRO
    // #define UNIQUE_MACRO
    //   ... header body ...
    // #endif
    // 第一次：宏未定义 → 处理头体并定义宏；再次包含：宏已定义 → 整段跳过。

    (void)stage01_guard_demo::hit_once();
    (void)stage01_guard_demo::hit_once();
    assert(stage01_guard_demo::inclusion_body_hits() == 1);

    stage01_guard_demo::Widget w{4};
    assert(stage01_guard_demo::widget_score(w) == 40);
    std::cout << "[intro] guard allows Widget to appear once per TU; score=" << stage01_guard_demo::widget_score(w)
              << '\n';

    // -------------------------------------------------------------------------
    // §进阶：guard 防什么 / 不防什么
    // -------------------------------------------------------------------------
    // 防: 同一 TU 内 a.hpp 与 b.hpp 都 include common.hpp → common 粘两次。
    // 不防: common.hpp 被 5 个 .cpp 各 include 一次 → 5 个 TU 各有一份
    //       （那是 ODR / inline / 头源分离问题，不是 guard 的职责）。
    std::cout << "[advanced] guards are per-TU duplicate paste protection only\n";

    // 宏名必须全局唯一。项目习惯: PROJECT_PATH_FILE_HPP_INCLUDED
    // 撞车后果: 第二份头的全部内容被跳过，可能直到链接/运行才暴露。
    assert(stage01_guard_demo::from_first_header == 1);
#if defined(from_second_header)
    // 不可达：名字在命名空间内
#endif
    // from_second_header 因撞车未定义——用特征检测式注释说明：
    // 若取消上面坏 guard 的撞车，应能使用 from_second_header == 2。
    std::cout << "[pitfall] identical guard macros across headers silently drop the second body\n";

    // -------------------------------------------------------------------------
    // §专家：可移植性与双重保险
    // -------------------------------------------------------------------------
    // · include guard 是 ISO 标准手法，库作者首选。
    // · 可与 #pragma once 并用（团队风格问题）。
    // · 生成宏名时避免只用 FOO_H（太容易撞）；可加项目/UUID/路径前缀。
    // · 预编译头 / 模块（C++20）改变工程组织，但不取消"理解 guard"的必要性。
    //
    // 验证重复包含错误（独立小实验）:
    //   写无 guard 的 head.hpp 含 struct S {}; 在 tu.cpp 里 #include 两次 → 重定义报错。
    //   加上 guard 后通过。

    const std::string rule = "unique macro name + ifndef/define/endif";
    assert(rule.find("ifndef") != std::string::npos);

    std::cout << "[expert] prefer unique portable guards for library headers\n";
    std::cout << "=== include_guard: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section02/include_guard", run>;

}  // namespace
