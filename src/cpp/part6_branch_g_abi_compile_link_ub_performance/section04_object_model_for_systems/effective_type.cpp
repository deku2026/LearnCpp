// LearnCpp topic
// Doc      : 第6部分-支线G · G8.2 effective type
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : effective_type
// Topic id : part6/g/section04/effective_type
//
// 要点: C 有 effective type; C++ 用对象模型 + 类型别名规则。
//       读写须通过合法类型。
// 参考: C99 6.5; C++ [basic.lval]

#include "learn/topic_registry.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <new>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G8 effective type / aliasing model ===\n";

    // C++: 对象有类型; 创建 int 对象后通过 int* 访问
    int x = 42;
    [[maybe_unused]] int* p = &x;
    assert(*p == 42);

    // 原始存储上创建对象；标量无具名析构函数写法 y->~int() 非法，用 destroy_at
    alignas(int) unsigned char buf[sizeof(int)];
    int* y = new (buf) int{7};
    assert(*y == 7);
    std::destroy_at(y);

    // 通过 memcpy 传输对象表示 (不改变"有效类型"违规)
    int a = 99;
    int b;
    std::memcpy(&b, &a, sizeof a);
    assert(b == 99);

    std::cout << "  C effective type ≈ dynamic type of last store to storage\n";
    std::cout << "  C++: prefer explicit lifetime (construct / start_lifetime_as)\n";
    std::cout << "effective_type: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/effective_type", run>;

}  // namespace
