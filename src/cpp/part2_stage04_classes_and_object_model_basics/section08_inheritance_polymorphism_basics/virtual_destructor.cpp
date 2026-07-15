// Topic     : 虚析构函数
// Doc       : 第2部分-阶段4 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/language/destructor
//
// 要点: 经基类指针 delete 派生对象时，基类析构必须 virtual，否则 UB/只调基类析构。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

namespace {

std::string g_log;

struct BaseVirt {
    virtual ~BaseVirt() { g_log += "~B"; }
};

struct DerivedV : BaseVirt {
    explicit DerivedV() { g_log += "+D"; }
    ~DerivedV() override { g_log += "~D"; }
};

// 非虚析构：仅作对照说明，不执行 delete 基类指针（避免真实 UB）
struct BaseNonVirt {
    ~BaseNonVirt() { g_log += "~NB"; }
};

struct DerivedNV : BaseNonVirt {
    ~DerivedNV() { g_log += "~ND"; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [virtual_destructor] ===\n";

    // -------------------------------------------------------------------------
    // §入门：virtual 析构正确链
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        BaseVirt* p = new DerivedV;
        delete p;  // ~D then ~B
    }
    assert(g_log == "+D~D~B");
    std::cout << "[intro] log=" << g_log << '\n';

    // -------------------------------------------------------------------------
    // §进阶：unique_ptr 默认要求完整删除器语义
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        std::unique_ptr<BaseVirt> up = std::make_unique<DerivedV>();
    }
    assert(g_log == "+D~D~B");
    std::cout << "[advanced] unique_ptr path log=" << g_log << '\n';

    // -------------------------------------------------------------------------
    // §专家：非虚对照（不经基类指针 delete）
    // -------------------------------------------------------------------------
    g_log.clear();
    {
        DerivedNV stack;
    }
    assert(g_log == "~ND~NB");  // 栈对象按完整类型析构，顺序正确

    // 危险反例（不要在生产跑 ASan 外的侥幸）：
    // BaseNonVirt* p = new DerivedNV; delete p; // 可能只 ~NB → 泄漏/UB

    // 规则：多态基类 → public virtual 析构，或 protected 非虚（禁止基类指针 delete）
    std::cout << "[expert] stack non-virt ok; never delete derived via non-virt base*\n";
    std::cout << "=== virtual_destructor: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section08/virtual_destructor", run>;

}  // namespace
