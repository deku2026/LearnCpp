// Topic    : C++20 指定初始化器 designated initializers
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 9.3
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section09_aggregates_and_struct_basics
// Item     : designated_initializers_cpp20
// Topic id : part2/stage02/section09/designated_initializers_cpp20
// Refs     : https://en.cppreference.com/cpp/language/aggregate_initialization
//            提案 P0329

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

struct ScreenConfig {
    bool autoScale;
    bool fullscreen;
    int bits;
    int planes;
};

struct Config {
    int width;
    int height;
    bool vsync;
    int samples;
    const char* title;
};

// 非聚合：不能用指定初始化器
struct HasCtor {
    int x;
    HasCtor(int v) : x(v) {}
};

union Slot {
    int i;
    double d;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [designated_initializers_cpp20] 入门：按成员名初始化 ===\n";
    {
        // 旧：靠位置，可读性差
        [[maybe_unused]] ScreenConfig a{true, false, 8, 1};

        // C++20：自解释
        [[maybe_unused]] ScreenConfig b{.autoScale = true, .fullscreen = false, .bits = 8, .planes = 1};
        [[maybe_unused]] ScreenConfig c{
            .autoScale = true, .fullscreen = false, .bits = 16, .planes = 0};  // 省略的值初始化
        (void)c;

        assert(a.autoScale && !a.fullscreen && a.bits == 8 && a.planes == 1);
        assert(b.bits == 8 && b.planes == 1);
        assert(c.autoScale && !c.fullscreen && c.bits == 16 && c.planes == 0);

        std::cout << "[intro] named designators improve readability\n";
    }

    std::cout << "=== 进阶：必须按声明顺序；省略成员值初始化 ===\n";
    {
        [[maybe_unused]] Config cfg{.width = 1920, .height = 1080, .vsync = true, .samples = 0, .title = "demo"};
        (void)cfg;
        // samples 省略 → 0
        assert(cfg.width == 1920 && cfg.height == 1080);
        assert(cfg.vsync && cfg.samples == 0);
        assert(std::string{cfg.title} == "demo");

        // 规则（违反则编译错误，仅注释示意）：
        // Config bad1{.height = 1, .width = 2};           // ❌ 乱序
        // Config bad2{1920, .height = 1080};              // ❌ 混用指定/非指定
        // Config bad3{.width = 1, .width = 2};            // ❌ 重复
        // 只能用于聚合：HasCtor h{.x = 1};                // ❌

        // 与 C 的区别：C 允许乱序与数组 [i]=；C++20 都不允许
        std::cout << "[advanced] C++ requires declaration order; no mix/repeat\n";
    }

    std::cout << "=== 专家：union 只能指定一个；嵌套指定 ===\n";
    {
        [[maybe_unused]] Slot s{.i = 42};
        assert(s.i == 42);
        // Slot s2{.i = 1, .d = 2.0};  // ❌ union 只能一个指示符

        struct Inner {
            int u;
            int v;
        };
        struct Outer {
            Inner in;
            int w;
        };
        [[maybe_unused]] Outer o{.in = {.u = 1, .v = 2}, .w = 3};
        assert(o.in.u == 1 && o.in.v == 2 && o.w == 3);

        // 配置对象验收：部分字段 + 默认零
        [[maybe_unused]] Config mini{.width = 800, .height = 600, .vsync = false, .samples = 0, .title = "mini"};
        (void)mini;
        assert(!mini.vsync && mini.samples == 0);

        std::cout << "[expert] nested designators; union single active member\n";
    }

    std::cout << "[designated_initializers_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section09/designated_initializers_cpp20", run>;

}  // namespace
