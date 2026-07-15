// Topic     : 声明 vs 定义
// Doc       : 第2部分-阶段1 · 步骤 4.1–4.2
// cppreference: https://en.cppreference.com/cpp/language/declarations
//               https://en.cppreference.com/cpp/language/definition
//
// 要点: 声明引入名字；定义提供实体；口诀"定义必是声明"；
//       extern / 前向声明 / 函数原型；类内 static 与 inline static。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

// —— 函数：声明（非定义）——
int scale(int x);  // 原型：承诺存在，定义在下方

// —— 变量：extern 仅声明 ——
extern int g_units;  // 不分配（在此 TU 稍后定义）

// —— 类型：前向声明 ——
struct Parcel;  // 不完整类型：可声明指针/引用，不可按值使用成员

// 需要完整类型的 API 只在定义之后
int parcel_weight(const Parcel& p);

struct Parcel {
    int grams = 0;
    std::string dest;
};

int parcel_weight(const Parcel& p) {
    return p.grams;
}

// 变量定义（分配存储）
int g_units = 3;

// 函数定义
int scale(int x) {
    return x * g_units;
}

// 类内 static：非 inline 成员是声明；inline static 可就地定义（C++17）
struct Depot {
    static int open_count;               // 声明
    inline static int closed_count = 0;  // 定义（允许头中出现）
    static int next_id() {               // 类内函数定义 → 隐式 inline
        return ++open_count;
    }
};

int Depot::open_count = 0;  // 类外定义

// const 默认内部链接：头中 `const int K = 1;` 各 TU 各一份副本（常见模式）
// 若要跨 TU 同一实体: extern const int K; 并在一处定义。
const int k_local_const = 5;

namespace {

// 不完整类型用途：打破循环依赖 / pimpl
struct HiddenImpl;
struct Handle {
    // 只持有指针时，头文件里前向声明 HiddenImpl 即可（真实工程）
    HiddenImpl* p = nullptr;
};

struct HiddenImpl {
    int secret = 99;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [declaration_vs_definition] ===\n";

    // -------------------------------------------------------------------------
    // §入门：口诀与最小例子
    // -------------------------------------------------------------------------
    // 声明: 告诉编译器"有这个名字，类型/种类是什么"
    // 定义: 把实体真正提供出来（函数体、对象存储、类成员列表...）
    // 口诀: 定义一定是声明；声明不一定是定义。

    assert(scale(4) == 12);
    assert(g_units == 3);

    Parcel p{};
    p.grams = 500;
    p.dest = "SH";
    assert(parcel_weight(p) == 500);
    std::cout << "[intro] scale(4)=" << scale(4) << " parcel_weight=" << p.grams << '\n';

    // -------------------------------------------------------------------------
    // §进阶：哪些"只声明"
    // -------------------------------------------------------------------------
    // · 函数原型（无函数体）
    // · extern 变量（无初始化器）
    // · 类/结构体前向声明
    // · 非 inline 静态数据成员的类内出现
    // · 模板的声明（定义可在别处，规则更复杂——阶段 6）

    assert(Depot::next_id() == 1);
    assert(Depot::next_id() == 2);
    Depot::closed_count += 1;
    assert(Depot::closed_count == 1);
    assert(k_local_const == 5);
    std::cout << "[advanced] extern/static/forward-declare patterns OK\n";

    // 不完整类型：可作指针
    Handle h{};
    HiddenImpl impl{};
    h.p = &impl;
    assert(h.p->secret == 99);
    std::cout << "[advanced] incomplete type enables pointer/pimpl decoupling\n";

    // -------------------------------------------------------------------------
    // §专家：为何分离？与翻译模型的契约
    // -------------------------------------------------------------------------
    // 每个 TU 独立编译 → 需要**看见声明**才能类型检查与生成调用代码；
    // 链接阶段再把调用点与**唯一定义**缝合。
    // 因此工程约定: 声明进头；非 inline 定义进单一 .cpp。
    //
    // 对照表:
    //   int f(int);           声明
    //   int f(int){...}       定义
    //   extern int g;         声明
    //   int g = 1;            定义
    //   struct S;             声明
    //   struct S { int n; };  定义
    //
    // 误区: "声明在 .h、定义在 .cpp" 不是语法强制，而是 ODR + 分离编译下的纪律。
    // 违反时可能: 编译过但链接 undefined reference / multiple definition，或 ODR-UB。

    std::cout << "[expert] declarations enable per-TU compilation; definitions must meet ODR\n";
    std::cout << "=== declaration_vs_definition: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/declaration_vs_definition", run>;

}  // namespace
