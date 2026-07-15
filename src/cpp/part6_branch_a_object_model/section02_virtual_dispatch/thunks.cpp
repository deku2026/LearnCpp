// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A5 thunk / this 调整)
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : thunks
// Topic id : part6/a/section02/thunks
//
// 要点: 经非主基类虚调用时, this 指向该基类子对象, 但覆盖函数需要完整对象 this;
//       ABI 在次 vtable 槽放入 thunk: 先调整 this (常为固定偏移), 再跳到最终函数。
//       本文件用“可观察的 this 地址”安全演示调整, 不解析原始 vtable 字节。
// 参考: Itanium non-virtual thunks (_ZThn...); [class.mi]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

namespace {

// 记录最近一次虚调用看到的 this (转成完整对象后的地址)
std::uintptr_t g_last_this_as_complete = 0;
std::string g_last_path;

struct Left {
    int left_tag = 0x11;
    virtual ~Left() = default;
    virtual void touch() {
        g_last_path = "Left::touch";
        // this 即 Left* — 对 Left 对象而言就是完整对象
        g_last_this_as_complete = reinterpret_cast<std::uintptr_t>(this);
    }
};

struct Right {
    int right_tag = 0x22;
    virtual ~Right() = default;
    virtual void touch() {
        g_last_path = "Right::touch";
        g_last_this_as_complete = reinterpret_cast<std::uintptr_t>(this);
    }
};

struct Both : Left, Right {
    int both_tag = 0x33;

    void touch() override {
        // 覆盖后只有一份 Both::touch; 无论经 Left* 还是 Right* 调用,
        // 进入函数体时 this 必须已是 Both* (由 thunk 或主 vtable 保证)。
        g_last_path = "Both::touch";
        g_last_this_as_complete = reinterpret_cast<std::uintptr_t>(this);
        assert(both_tag == 0x33);
        assert(left_tag == 0x11);
        assert(right_tag == 0x22);
    }
};

// 仅 Right 侧覆盖: 强制次路径走调整
struct OnlyRightOverride : Left, Right {
    int payload = 99;
    void touch() override {
        // 此 touch 同时覆盖 Left::touch 与 Right::touch (同签名合并)
        g_last_path = "OnlyRightOverride::touch";
        g_last_this_as_complete = reinterpret_cast<std::uintptr_t>(this);
        assert(payload == 99);
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== thunks / this-adjustment (MI) ===\n";

    Both b;
    const auto both_addr = reinterpret_cast<std::uintptr_t>(&b);
    Left* lp = &b;
    Right* rp = &b;

    const auto left_delta = static_cast<std::ptrdiff_t>(reinterpret_cast<std::uintptr_t>(lp) - both_addr);
    const auto right_delta = static_cast<std::ptrdiff_t>(reinterpret_cast<std::uintptr_t>(rp) - both_addr);

    std::cout << "  Left*  delta from Both: " << left_delta << '\n';
    std::cout << "  Right* delta from Both: " << right_delta << '\n';
    assert(left_delta == 0);
    assert(right_delta != 0);

    // --- 经主基类调用: 通常无需 thunk, this 已是完整对象 ---
    g_last_path.clear();
    g_last_this_as_complete = 0;
    lp->touch();
    assert(g_last_path == "Both::touch");
    assert(g_last_this_as_complete == both_addr);

    // --- 经次基类调用: ABI 插入 thunk, 把 this 从 Right* 调回 Both* ---
    // 用户代码仍写 rp->touch(); 汇编里可能是:
    //   this' = this - right_delta;  jmp Both::touch
    // (Itanium 符号名常含 _ZThn<N>_ ... "non-virtual thunk, adjust by N")
    g_last_path.clear();
    g_last_this_as_complete = 0;
    rp->touch();
    assert(g_last_path == "Both::touch");
    assert(g_last_this_as_complete == both_addr);  // 已调整, 不是 Right 子对象址

    // 若错误地以 Right* 的位模式当 Both* 用, both_tag 会读到垃圾 — 切勿手动改。
    std::cout << "  after Right* call, this inside final fn == &Both: yes\n";

    // --- 再验证: 覆盖函数里能安全访问所有子对象成员 ---
    OnlyRightOverride o;
    const auto o_addr = reinterpret_cast<std::uintptr_t>(&o);
    Right* orp = &o;
    Left* olp = &o;
    orp->touch();
    assert(g_last_this_as_complete == o_addr);
    olp->touch();
    assert(g_last_this_as_complete == o_addr);

    // 虚继承下偏移编译期不可固定 → 用 vcall offset 槽, thunk 多一次内存读
    // (见 virtual_inheritance_diamond topic; 此处只作注释对照)

    std::cout << "thunks: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/thunks", run>;

}  // namespace
