// LearnCpp topic
// Doc      : part6-branch-c-memory-management.md (C5 start_lifetime_as)
// Stage    : part6_branch_c_memory_management
// Section  : section04_lifetime_routines
// Item     : start_lifetime_as_cpp23
// Topic id : part6/c/section04/start_lifetime_as_cpp23
//
// 要点: 只开始生命周期、不调用构造、保留字节——把缓冲当隐式生命周期类型读。
// new 三件事中的「③」独立出来。
// 参考: https://en.cppreference.com/w/cpp/memory/start_lifetime_as

#include "learn/topic_registry.hpp"

#include <cassert>
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
    auto* p = std::launder(reinterpret_cast<Point*>(bytes));
    assert(p->x == 1.5f && p->y == 2.5f);
    std::cout << "  start_lifetime_as unavailable; launder+memcpy demo\n";
#endif

    // 三件套对照:
    // placement new / construct_at = 构造 + 开始生命（可非平凡）
    // start_lifetime_as            = 仅开始生命（隐式生命周期类型）
    // launder                      = 指针消毒，不创建对象
    // ❌ std::string 不可 start_lifetime_as
    std::cout << "  requires implicit-lifetime type (see is_implicit_lifetime)\n";
    std::cout << "start_lifetime_as_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section04/start_lifetime_as_cpp23", run>;

}  // namespace
