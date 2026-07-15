// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B1 生命周期阶段)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : initialization_phases
// Topic id : part6/b/section01/initialization_phases
//
// 要点: 分配 → 构造(生命开始) → 使用 → 析构(生命结束) → 释放；
//       生命周期外访问 = UB。用 placement new / destroy_at 分阶段实证。
// 参考: [basic.life]

#include "learn/topic_registry.hpp"

#include <cassert>
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
        id = -1;  // 析构开始后状态失效（仅教学标记）
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B1 initialization / lifetime phases ===\n";

    // 1) 分配原始存储（尚无对象生命周期）
    alignas(Phase) unsigned char storage[sizeof(Phase)];

    // 2) 构造 → 生命周期开始
    Phase* p = std::construct_at(reinterpret_cast<Phase*>(storage), "widget", 42);
    assert(p->id == 42);
    assert(p->name == "widget");

    // 3) 使用（对象活着）
    p->id = 7;
    assert(p->id == 7);

    // 4) 析构 → 生命周期结束（此后不得访问成员）
    std::destroy_at(p);
    // p->id;  // ❌ UB：生命周期外访问——禁止

    // 5) 存储可复用：再次构造新对象
    p = std::construct_at(reinterpret_cast<Phase*>(storage), "reuse", 99);
    assert(p->id == 99);
    std::destroy_at(p);

    // 自动存储：编译器自动完成 1–5
    {
        Phase local{"auto", 1};
        assert(local.id == 1);
    }

    // 动态存储：new = 分配+构造；delete = 析构+释放
    Phase* heap = new Phase{"heap", 2};
    assert(heap->id == 2);
    delete heap;

    std::cout << "initialization_phases: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/initialization_phases", run>;

}  // namespace
