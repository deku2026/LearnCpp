// Topic     : 头文件 / 源文件分离
// Doc       : 第2部分-阶段1 · 步骤 5.1–5.2
// cppreference: https://en.cppreference.com/cpp/language/definition
// Core Guidelines: SF.* source file organization
//
// 要点: 声明进头、定义进源的推导链；inline/模板例外；
//       单文件内模拟 math_utils 多文件布局；手动构建命令。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>

// =============================================================================
// 以下模拟 math_utils.hpp —— 真实工程会拆到独立头并加 include guard
// =============================================================================
#ifndef LEARNCPP_STAGE01_MATH_UTILS_MINI_HPP
#define LEARNCPP_STAGE01_MATH_UTILS_MINI_HPP

namespace mu {

// 非 inline：头中**只声明**
int add(int a, int b);

// 小函数：定义可在头中，必须 inline（或当模板/类内成员）
inline int square(int x) {
    return x * x;
}

// 类定义本身允许多 TU（ODR③），成员函数类内定义 ⇒ 隐式 inline
struct Accumulator {
    int value = 0;
    void push(int x) { value += x; }
    int get() const { return value; }
};

}  // namespace mu

#endif
// =============================================================================
// 以下模拟 math_utils.cpp —— 唯一定义落点
// =============================================================================
namespace mu {
int add(int a, int b) {
    return a + b;
}
}  // namespace mu

// =============================================================================
// 使用方（模拟 main.cpp）
// =============================================================================

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [header_source_separation] ===\n";

    // -------------------------------------------------------------------------
    // §入门：推导链
    // -------------------------------------------------------------------------
    // 每个 .cpp 独立编译
    //   → TU 需要看见所用实体的【声明】
    //     → 声明集中放头，#include 粘贴
    //   → 【定义】非 inline 只能有一份
    //     → 放进唯一的某个 .cpp
    assert(mu::add(3, 4) == 7);
    assert(mu::square(5) == 25);
    mu::Accumulator acc{};
    acc.push(10);
    acc.push(5);
    assert(acc.get() == 15);
    std::cout << "[intro] mu::add(3,4)=" << mu::add(3, 4) << " square(5)=" << mu::square(5) << " acc=" << acc.get()
              << '\n';

    // -------------------------------------------------------------------------
    // §进阶：什么可以进头
    // -------------------------------------------------------------------------
    // 适合头:
    //   · 函数/变量声明、类定义、枚举、using 别名
    //   · inline 函数/变量、模板定义、constexpr 小工具
    // 适合源:
    //   · 非 inline 函数定义、非 inline 全局/静态成员定义、隐藏实现细节
    // 反模式:
    //   · 头里 using namespace std;
    //   · 头里非 inline 函数定义（多 TU include 即炸）
    std::cout << "[advanced] headers: decls + classes + inline/templates; sources: the one definition\n";

    // -------------------------------------------------------------------------
    // §专家：手动构建验收（路线图验收①）
    // -------------------------------------------------------------------------
    // 真实拆分后:
    //   g++ -std=c++23 -Wall -c math_utils.cpp -o math_utils.o
    //   g++ -std=c++23 -Wall -c main.cpp -o main.o
    //   g++ math_utils.o main.o -o app
    // MSVC:
    //   cl /std:c++latest /EHsc /c math_utils.cpp
    //   cl /std:c++latest /EHsc /c main.cpp
    //   link math_utils.obj main.obj /out:app.exe
    //
    // 源文件应 #include 自己的头，让编译器核对声明与定义签名一致。
    // pimpl: 头里只前向声明 Impl，指针成员；完整 Impl 定义藏在 .cpp → 降编译依赖、稳 ABI。
    //
    // 本 learn_cpp 把"头+源"折叠进单一 TU 以便注册运行，但规则与多文件工程相同。

    const std::string checklist = "declare in header / define once in source / include own header from source";
    assert(checklist.find("define once") != std::string::npos);

    std::cout << "[expert] " << checklist << '\n';
    std::cout << "=== header_source_separation: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/header_source_separation", run>;

}  // namespace
