// LearnCpp topic
// Doc      : 第6部分-支线G · G9.2 隐式对象创建
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : implicit_object_creation_cpp20
// Topic id : part6/g/section04/implicit_object_creation_cpp20
//
// 要点: C++20 起, 某些操作可隐式创建隐式生命周期类型对象
//       (如 malloc 返回的存储上使用)。
// 参考: P0593

#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <new>

namespace {

struct Pod {
    int x;
    int y;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G9 implicit object creation (C++20) ===\n";

    // 分配原始存储
    void* raw = std::malloc(sizeof(Pod));
    assert(raw);

    // C++20: 将存储视为可隐式创建 Pod (隐式生命周期类型)
    // 通过 start 或 placement — 这里用 placement 明确创建 (可移植教学)
    Pod* p = new (raw) Pod{3, 4};
    assert(p->x == 3 && p->y == 4);
    p->~Pod();
    std::free(raw);

    // memcpy 到合适对齐存储可隐式创建 (P0593 场景)
    alignas(Pod) unsigned char buf[sizeof(Pod)];
    Pod src{1, 2};
    std::memcpy(buf, &src, sizeof src);
    Pod dst{};
    std::memcpy(&dst, buf, sizeof dst);
    assert(dst.x == 1 && dst.y == 2);

    std::cout << "  P0593: malloc/memcpy/etc. can create implicit-lifetime objects\n";
    std::cout << "  still prefer explicit construct / start_lifetime_as when unsure\n";
    std::cout << "implicit_object_creation_cpp20: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/implicit_object_creation_cpp20", run>;

}  // namespace
