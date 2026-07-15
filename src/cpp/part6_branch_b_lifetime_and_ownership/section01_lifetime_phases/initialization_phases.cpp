// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B1 生命周期阶段)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : initialization_phases
// Topic id : part6/b/section01/initialization_phases
//
// 要点: 分配 → 构造(生命周期开始) → 使用 → 析构(生命周期结束) → 释放；
//       在生命周期外访问 = UB。用 placement / construct_at / destroy_at 分阶段实证。
// 参考: [basic.life]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <string>

namespace {

struct Phase {
    std::string name;
    int id = 0;
    explicit Phase(std::string n, int i) : name(std::move(n)), id(i) {
        std::cout << "  construct " << name << " id=" << id << '\n';
    }
    ~Phase() {
        std::cout << "  destroy " << name << " id=" << id << '\n';
        id = -1;  // 析构后状态无效（教学标记，勿在外部读）
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B1 initialization / lifetime phases ===\n";

    // --- 入门: 手动五阶段 ---
    // 1) 分配原始存储（尚无对象生命周期）
    alignas(Phase) unsigned char storage[sizeof(Phase)];

    // 2) 构造 → 生命周期开始
    Phase* p = std::construct_at(reinterpret_cast<Phase*>(storage), "widget", 42);
    assert(p->id == 42);
    assert(p->name == "widget");

    // 3) 使用（生命周期内）
    p->id = 7;
    assert(p->id == 7);

    // 4) 析构 → 生命周期结束；此后不得访问成员
    std::destroy_at(p);
    // p->id;  // ❌ UB（生命周期外）；故意省略

    // 5) 存储可复用：再次构造新对象
    p = std::construct_at(reinterpret_cast<Phase*>(storage), "reuse", 99);
    assert(p->id == 99);
    assert(p->name == "reuse");
    std::destroy_at(p);

    // --- 进阶: 自动 / 动态 存储把阶段绑在一起 ---
    {
        Phase local{"auto", 1};  // 分配+构造；离开作用域析构+释放
        assert(local.id == 1);
    }

    Phase* heap = new Phase{"heap", 2};  // 分配+构造
    assert(heap->id == 2);
    delete heap;  // 析构+释放

    // --- 专家: 与 start_lifetime_as / launder 分界（支线 C/G）---
    // placement/construct_at: 真正运行构造函数，开始生命周期
    // start_lifetime_as: 对隐式生命周期类型「只开始生命周期」，不跑构造
    // launder: 指针消毒，不创建对象
    std::cout << "  outside lifetime access = UB (before ctor / after dtor start)\n";
    std::cout << "  construct_at = begin life; destroy_at = end life\n";
    std::cout << "initialization_phases: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/initialization_phases", run>;

}  // namespace
