// Topic     : ODR（一处定义原则）入门
// Doc       : 第2部分-阶段1 · 步骤 4.3–4.5
// cppreference: https://en.cppreference.com/cpp/language/definition
//               https://en.cppreference.com/cpp/language/inline
// ISO       : [basic.def.odr]
//
// 要点: TU 内至多一份；程序内非 inline odr-use 恰一份；类/inline/模板可多 TU
//       但必须相同；inline 放宽 ODR 而非强制内联；违规常为 NDR UB。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

// —— 规则③ 实体：允许每个 TU 一份相同定义（本文件仅一个 TU，演示形态）——
struct Point {
    int x = 0;
    int y = 0;
};

// inline 函数：允许出现在多个 TU（若放在头中被多方 include）
inline int manhattan(Point a, Point b) {
    auto abs_i = [](int v) { return v < 0 ? -v : v; };
    return abs_i(a.x - b.x) + abs_i(a.y - b.y);
}

// C++17 inline 变量：头文件中的全局实体合法共享
inline int g_path_queries = 0;

// 非 inline 函数：整个程序只能有一份定义（本 TU 这一份）
int origin_distance(Point p) {
    ++g_path_queries;
    return manhattan(p, Point{});
}

// 模板：实例化规则与 ODR 交织（阶段 6）；此处仅点到"定义通常在头中"
template <class T>
constexpr T twice(T v) {
    return v + v;
}

namespace {

// 模拟"两个 TU 拷贝了相同的 inline 定义"——链接器会合并为一
[[maybe_unused]] inline int shared_ticket() {
    static int n = 0;  // 注意: inline 函数内 static 局部跨 TU 仍是同一实体
    return ++n;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [one_definition_rule_intro] ===\n";

    // -------------------------------------------------------------------------
    // §入门：三条 ODR（教学压缩版）
    // -------------------------------------------------------------------------
    // ① 任一 TU 内：变量/函数/类/枚举/概念/模板等至多一个定义。
    // ② 整个程序：每个非 inline 且被 odr-use 的函数/变量恰好一个定义。
    // ③ 类、inline 函数/变量、模板等：可在多 TU 各有定义，但必须 token 级相同。

    Point a{1, 5};
    Point b{4, 1};
    assert(manhattan(a, b) == 7);
    assert(origin_distance(a) == 6);
    assert(g_path_queries == 1);
    assert(twice(21) == 42);
    std::cout << "[intro] manhattan=" << manhattan(a, b) << " origin_distance=" << origin_distance(Point{3, 4}) << '\n';

    // -------------------------------------------------------------------------
    // §进阶：inline 的现代含义
    // -------------------------------------------------------------------------
    // inline ≠ "请编译器一定内联展开"。
    // inline ≈ "允许相同定义出现在多个 TU，链接时合并为同一实体"（放宽 ②）。
    // 是否内联展开由优化器决定，与有无 inline 关键字基本脱钩。
    assert(shared_ticket() == 1);
    assert(shared_ticket() == 2);
    std::cout << "[advanced] inline relaxes ODR for multi-TU identical definitions\n";

    // 类内定义的成员函数隐式 inline——故类定义可放头中。
    struct Tiny {
        int plus1(int v) const { return v + 1; }
    };
    assert(Tiny{}.plus1(41) == 42);

    // -------------------------------------------------------------------------
    // §专家：odr-use、违规形态与防御
    // -------------------------------------------------------------------------
    // 「odr-use」粗义：以需要定义/地址/真身的方式使用实体（调用函数、取地址、
    //  绑定引用到对象等）。仅出现在不求值语境（如某些 decltype/sizeof 形态）
    //  可能不算 odr-use——完整规则见 [basic.def.odr]；教学先记「用到了真身」。
    //
    // A) 头文件放非 inline 函数定义，被两个 .cpp include → multiple definition
    //    （链接器常能诊断）。
    // B) 两个 TU 手写同名 class，布局/成员不一致 → ODR 违规，**不要求诊断 (NDR)**，
    //    属 UB：可能静默错算、崩溃、"换编译器才炸"。
    // C) inline 函数在两 TU 中"看起来同名但体不同"（含宏导致）→ 同为 UB。
    //
    // 防御:
    //   · 单一真相来源：类/inline/模板放公共头，勿复制粘贴定义。
    //   · 普通函数/全局非 inline 变量：声明在头，定义在唯一 .cpp。
    //   · TU 私有助手进匿名命名空间，避免跨 TU 撞名。
    //
    // 实证（独立小工程）:
    //   utils.hpp 定义非 inline add；a.cpp/b.cpp 都 include 并调用；链接看 multiple definition。
    //   给 add 加 inline 后链接通过——肌肉记忆：inline 放宽 ODR。
    //   nm -C a.o b.o：非 inline 时两边都有 T add；加 inline 后链接器合并。

    // 本 TU 内：非 inline origin_distance 被调用 → 典型 odr-use，本文件必须提供定义
    assert(origin_distance(Point{0, 0}) == 0);
    // inline 实体可多 TU 各一份相同定义；此处共享 static 局部演示「同一实体」
    assert(shared_ticket() == 3);

    const std::string rule2 = "non-inline odr-used entities: exactly one definition in the program";
    assert(rule2.find("exactly one") != std::string::npos);
    assert(rule2.find("odr-used") != std::string::npos);

    const std::string rule3 = "class/inline/template: multi-TU OK iff definitions are token-identical";
    assert(rule3.find("token-identical") != std::string::npos);

    std::cout << "[expert] ODR violation can be NDR UB — keep one identical definition source\n";
    std::cout << "=== one_definition_rule_intro: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/one_definition_rule_intro", run>;

}  // namespace
