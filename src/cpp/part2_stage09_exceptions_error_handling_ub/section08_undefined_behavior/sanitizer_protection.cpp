// Topic    : Sanitizer 三件套护栏：ASan / UBSan / TSan
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 11.4
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : sanitizer_protection
// Topic id : part2/stage09/section08/sanitizer_protection
// Refs     : https://github.com/google/sanitizers/wiki/AddressSanitizer
//            https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
//            https://clang.llvm.org/docs/ThreadSanitizer.html

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

// 本 topic 演示“如何用安全 API 写代码 + 如何启用 sanitizer”，
// 故意不注入会失败的 UB，以便默认测试全绿。

void demo_safe_patterns() {
    // ASan 常抓：OOB / UAF / leak —— 这里全部用安全写法
    std::vector<int> v{1, 2, 3};
    assert(v.at(2) == 3);
    auto p = std::make_unique<int[]>(8);
    p[0] = 1;
    p.reset();

    // UBSan 常抓：有符号溢出 / 空解引用 / 对齐 —— 安全写法
    const int a = 100;
    [[maybe_unused]] const int b = 200;
    assert(a + b == 300);
    [[maybe_unused]] const int* q = &a;
    assert(q != nullptr && *q == 100);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [sanitizer_protection] 主干：安全代码路径 ===\n";
    demo_safe_patterns();
    std::cout << "safe patterns executed (should be silent under ASan/UBSan)\n";

    std::cout << "=== 对抗：启用命令备忘 ===\n";
    // Windows clang-cl ASan:
    //   clang-cl /std:c++latest /EHsc /W4 /fsanitize=address /Zi app.cpp
    // Linux GCC/Clang:
    //   clang++ -std=c++23 -fsanitize=address,undefined -g app.cpp && ./a.out
    // TSan（单独，勿与 ASan 同开）:
    //   clang++ -std=c++23 -fsanitize=thread -g app.cpp -pthread && ./a.out
    // CMake 本仓库：
    //   -DLEARNCPP_ENABLE_ASAN=ON
    //   -DLEARNCPP_ENABLE_UBSAN=ON  (clang/gcc)
    std::cout << "ASan : OOB, UAF, stack-use-after-return, leaks\n";
    std::cout << "UBSan: signed overflow, null deref, alignment, shift, ...\n";
    std::cout << "TSan : data races (alone, not with ASan)\n";

    std::cout << "=== 专节：纪律 ===\n";
    // 开发/测试构建默认开 ASan+UBSan 跑测试。
    // 生产构建关闭（开销大）。
    // strict aliasing 不能全靠 sanitizer —— 编码纪律 + bit_cast。
    std::cout << "dev builds: sanitize; prod: off; discipline > luck\n";

    // 标记“若你想本地验证 sanitizer 是否工作”，可手动临时取消注释危险代码，
    // 但不要提交进主线教学默认路径。
    // int boom[1]{}; (void)boom[1]; // ASan
    // int* n = nullptr; (void)*n;   // UBSan

    std::cout << "[sanitizer_protection] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/sanitizer_protection", run>;

}  // namespace
