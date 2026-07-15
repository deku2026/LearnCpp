// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C5 start_lifetime_as)
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_cpp23
// Topic id : part6/c/section04/start_lifetime_as_cpp23
//
// 要点: 只开始生命周期、不调构造、保留字节——把缓冲当隐式生命周期对象读。
// 参考: https://en.cppreference.com/w/cpp/memory/start_lifetime_as

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>

namespace {

struct Point {
    float x;
    float y;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== C5 start_lifetime_as (C++23) ===\n";

    alignas(Point) unsigned char bytes[sizeof(Point)];
    Point src{1.5f, 2.5f};
    std::memcpy(bytes, &src, sizeof(Point));

#if defined(__cpp_lib_start_lifetime_as) && __cpp_lib_start_lifetime_as >= 202207L
    Point* p = std::start_lifetime_as<Point>(bytes);
    assert(p->x == 1.5f);
    assert(p->y == 2.5f);
    std::cout << "  start_lifetime_as: lifetime without ctor, bytes kept\n";
#else
    // 可移植回退：对隐式生命周期类型，C++20 起某些分配隐式创建对象；
    // 教学上用 memcpy + launder 近似（实现定义边界时仅作演示）。
    auto* p = std::launder(reinterpret_cast<Point*>(bytes));
    assert(p->x == 1.5f && p->y == 2.5f);
    std::cout << "  start_lifetime_as unavailable; launder+memcpy demo\n";
#endif

    // 对比：非平凡类型必须 construct_at
    // std::string 不能 start_lifetime_as

    std::cout << "start_lifetime_as_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_cpp23", run>;

}  // namespace
