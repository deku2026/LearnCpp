// Topic     : 读未初始化自动变量是 UB
// Doc       : 第2部分-阶段2 · 步骤 2.6
// cppreference: https://en.cppreference.com/cpp/language/default_initialization
//               https://en.cppreference.com/cpp/language/ub
//
// 要点: 自动存储期内置类型默认初始化后值不确定；读取即 UB（C++23）；
//       不要演示“打印垃圾值”；用对照：声明即初始化 / {}。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <new>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [uninitialized_read_is_ub] ===\n";

    // -------------------------------------------------------------------------
    // 入门：铁律 — 声明即初始化
    // -------------------------------------------------------------------------
    // int x;           // 默认初始化，不确定值
    // int y = x + 1;   // ⚠️ UB：读了不确定值 —— 本文件故意不执行

    int x = 0;
    [[maybe_unused]] int y = x + 1;
    assert(y == 1);

    [[maybe_unused]] int z{};  // 值初始化
    assert(z == 0);
    std::cout << "[intro] always initialize; reading indeterminate is UB\n";

    // -------------------------------------------------------------------------
    // 进阶：哪些“看起来像未初始化”其实安全
    // -------------------------------------------------------------------------
    // 1) 静态 / 线程存储期：先零初始化
    // 2) 类类型默认构造后可能已有定义状态
    // 3) unsigned char / std::byte 的不确定值在部分场景可复制（窄例外）——仍别当正常数用

    [[maybe_unused]] static int static_zero;  // 零初始化
    assert(static_zero == 0);

    // new 默认初始化 vs 值初始化
    int* p = new int;    // *p 不确定 —— 不读
    int* q = new int{};  // 0
    assert(*q == 0);
    // 若必须占用存储但不读：可以 placement 或稍后赋值
    *p = 42;  // 写入后可读
    assert(*p == 42);
    delete p;
    delete q;
    std::cout << "[advanced] static-duration objects are zeroed; heap new T is not\n";

    // -------------------------------------------------------------------------
    // 专家：UB 的现实后果 + C++26 动向
    // -------------------------------------------------------------------------
    // 编译器可假设“程序永不触发 UB”，从而删除分支、误算优化。
    // ASan/MSan/Valgrind 可抓未初始化使用；养成 {} 习惯成本最低。
    // C++26：读未初始化局部从 UB 调整为 erroneous behaviour（仍错误，但模型不同）。
    // 学习基准仍是 C++23：当作 UB 处理。

    // 用 unsigned char 缓冲区演示“先写后读”
    alignas(int) unsigned char buf[sizeof(int)];
    // 读 buf 作为 int 仍可能是未初始化对象生命周期问题——正确做法：
    int value;
    std::memcpy(&value, buf, sizeof(buf));  // 仍依赖 buf 内容；应先填充
    std::memset(buf, 0, sizeof(buf));
    std::memcpy(&value, buf, sizeof(value));
    assert(value == 0);

    std::cout << "[expert] sanitizers catch this; prefer T x{} everywhere uncertain\n";
    std::cout << "=== uninitialized_read_is_ub: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/uninitialized_read_is_ub", run>;

}  // namespace
