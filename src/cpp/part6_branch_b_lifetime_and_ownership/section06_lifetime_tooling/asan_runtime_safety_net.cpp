// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B9 ASan / B12)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : asan_runtime_safety_net
// Topic id : part6/b/section06/asan_runtime_safety_net
//
// 要点: ASan 运行期抓悬垂；本文件展示「会被 ASan 抓」的模式与安全改写，
//       并打印如何启用 sanitizer（不故意触发 UAF）。
// 参考: https://clang.llvm.org/docs/AddressSanitizer.html

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B9 ASan runtime safety net ===\n";
    std::cout << "  enable (Windows clang-cl): /fsanitize=address /Zi\n";
    std::cout << "  enable (GCC/Clang):        -fsanitize=address -g -fno-omit-frame-pointer\n";
    std::cout << "  catches: heap-use-after-free, stack-use-after-scope, buffer-overflow\n";

    // 1 heap-use-after-free（安全对照）
    {
        auto p = std::make_unique<int>(1);
        p.reset();
        // *p; // ASan: heap-use-after-free
        assert(p == nullptr);
    }

    // 2 stack-use-after-scope
    {
        int* escaped = nullptr;
        {
            int local = 2;
            escaped = &local;
            assert(*escaped == 2);
        }
        // *escaped; // ASan: stack-use-after-scope
        escaped = nullptr;
        (void)escaped;
    }

    // 3 container invalidation
    {
        std::vector<int> v{1, 2, 3};
        // int* p = v.data(); v.push_back(...); *p; // 可能 UAF
        v.push_back(4);
        assert(v[0] == 1);
    }

    // 4 string_view 底层
    {
        std::string owner = "asan";
        [[maybe_unused]] std::string_view sv = owner;
        assert(sv == "asan");
        // owner 销毁后不要用 sv
    }

    // 5 对照表：形态 → ASan 典型报告
    std::cout << "  map (doc B3):\n";
    std::cout << "    dangling ref/pointer local → stack-use-after-scope\n";
    std::cout << "    use-after-delete / invalid iterator → heap-use-after-free\n";
    std::cout << "  limits: only executed paths; combine with tests/fuzzing\n";
    std::cout << "  multilayer: GSL types + clang-tidy + ASan\n";
    std::cout << "asan_runtime_safety_net: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/asan_runtime_safety_net", run>;

}  // namespace
