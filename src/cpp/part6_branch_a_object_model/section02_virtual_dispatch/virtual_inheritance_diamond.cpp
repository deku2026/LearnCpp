// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A6 虚继承 / 菱形)
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : virtual_inheritance_diamond
// Topic id : part6/a/section02/virtual_inheritance_diamond
//
// 要点: 非虚菱形 → 两份基类 (歧义 + 重复数据);
//       virtual 基类 → 共享一份; 代价: 对象往往更大、访问虚基类走运行期偏移 (vbase);
//       最派生类负责初始化虚基类。
// 参考: [class.mi] [class.base.init] Itanium vbase offset

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

namespace {

// ---------- 非虚菱形 ----------
struct CoreNV {
    int core = 1;
    virtual ~CoreNV() = default;
    virtual std::string id() const { return "CoreNV"; }
};

struct LeftNV : CoreNV {
    int left = 2;
};

struct RightNV : CoreNV {
    int right = 3;
};

struct DiamondNV : LeftNV, RightNV {
    int bottom = 4;
};

// ---------- 虚菱形 ----------
struct CoreV {
    int core = 1;
    virtual ~CoreV() = default;
    virtual std::string id() const { return "CoreV"; }
    explicit CoreV(int c = 1) : core(c) {}
};

struct LeftV : virtual CoreV {
    int left = 2;
    LeftV() : CoreV(100) {}  // 当 LeftV 是最派生时生效; 在 DiamondV 中被忽略
};

struct RightV : virtual CoreV {
    int right = 3;
    RightV() : CoreV(200) {}
};

struct DiamondV : LeftV, RightV {
    int bottom = 4;
    DiamondV() : CoreV(42) {}  // 最派生类初始化唯一虚基类
};

struct DiamondVDefault : LeftV, RightV {
    int bottom = 4;
    // 未显式初始化 CoreV → 用 CoreV 默认构造
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== virtual inheritance / diamond ===\n";

    // --- 非虚: 两份 Core, 访问歧义 ---
    DiamondNV nv;
    // nv.core = 1;  // 错误: 歧义
    nv.LeftNV::core = 11;
    nv.RightNV::core = 22;
    assert(nv.LeftNV::core == 11);
    assert(nv.RightNV::core == 22);
    assert(&nv.LeftNV::core != &nv.RightNV::core);

    // 两份 Core 子对象 → 通常比“逻辑一份”更大
    std::cout << "  sizeof(DiamondNV)=" << sizeof(DiamondNV) << " (two CoreNV subobjects)\n";

    // 转 CoreNV* 也歧义, 需显式路径
    [[maybe_unused]] CoreNV* c_left = static_cast<LeftNV*>(&nv);
    [[maybe_unused]] CoreNV* c_right = static_cast<RightNV*>(&nv);
    assert(c_left != c_right);

    // --- 虚继承: 一份 Core, 无歧义 ---
    DiamondV v;
    v.core = 7;  // OK, 唯一
    assert(v.core == 7);
    assert(v.left == 2 && v.right == 3 && v.bottom == 4);

    [[maybe_unused]] CoreV* cv = &v;  // 无歧义
    assert(cv->core == 7);
    assert(cv->id() == "CoreV");

    // 最派生类初始化虚基类: 构造里写的是 42, 随后我们改成了 7
    DiamondV v2;
    assert(v2.core == 42);  // DiamondV() : CoreV(42)

    // LeftV/RightV 里的 CoreV(100)/(200) 在 DiamondV 构造中不会决定最终 core
    DiamondVDefault vd;
    assert(vd.core == 1);  // 默认构造

    std::cout << "  sizeof(DiamondV)=" << sizeof(DiamondV) << " (one CoreV + vbase machinery)\n";
    std::cout << "  sizeof(LeftV)=" << sizeof(LeftV) << " sizeof(RightV)=" << sizeof(RightV) << '\n';

    // 虚继承对象往往 ≥ 非虚菱形 (多了偏移机制), 尽管“逻辑数据”少一份
    // 具体谁大取决于 ABI/成员; 只打印对照, 不断言大小次序的可移植性
    std::cout << "  compare NV vs V sizes: " << sizeof(DiamondNV) << " vs " << sizeof(DiamondV) << '\n';

    // 独立 LeftV: 仍含虚基类机制, 访问 core 正常
    LeftV alone;
    alone.core = 5;
    assert(alone.core == 5);
    // 同一 LeftV 类型, 独立时与嵌入 DiamondV 时到 CoreV 的偏移可以不同
    // → 编译器不能硬编码偏移, 要用 vtable 里的 vbase offset (运行期)

    // 共享验证: Left/Right 路径看到同一 core 地址
    [[maybe_unused]] LeftV* lv = &v2;
    [[maybe_unused]] RightV* rv = &v2;
    assert(&lv->core == &rv->core);
    assert(&lv->core == &v2.core);

    // 覆盖虚基类虚函数
    struct Core2 {
        virtual ~Core2() = default;
        virtual int f() const { return 1; }
    };
    struct L2 : virtual Core2 {
        int f() const override { return 2; }
    };
    struct R2 : virtual Core2 {};
    struct D2 : L2, R2 {};
    D2 d2;
    [[maybe_unused]] Core2* c2 = &d2;
    assert(c2->f() == 2);  // 共享一份, 覆盖生效

    std::cout << "virtual_inheritance_diamond: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/virtual_inheritance_diamond", run>;

}  // namespace
