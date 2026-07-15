// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B9 ASan)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section06_lifetime_tooling
// Item     : asan_runtime_safety_net
// Topic id : part6/b/section06/asan_runtime_safety_net
//
// 要点: ASan 是运行期兜底；本文件展示「会被 ASan 抓到的模式」的安全改写，
//       并打印如何启用 sanitizer（不主动触发 UAF）。
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
    std::cout << "  enable: clang/gcc -fsanitize=address / MSVC /fsanitize=address\n";
    std::cout << "  catches: heap-use-after-free, stack-use-after-scope, ...\n";

    // 模式对照（安全版）—— 真 UAF 留给专门 asan 复现作业
    // 1 heap-use-after-free
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
            assert(*escaped == 2);  // 仍在作用域内
        }
        // *escaped; // ASan: stack-use-after-scope
        escaped = nullptr;
        (void)escaped;
    }

    // 3 container invalidation
    {
        std::vector<int> v{1, 2, 3};
        v.push_back(4);
        // 使用新索引而非旧指针
        assert(v[0] == 1);
    }

    // 4 string_view 底层
    {
        std::string owner = "asan";
        std::string_view sv = owner;
        assert(sv == "asan");
        // owner 销毁后勿用 sv
    }

    std::cout << "  limits: only executed paths; use with tests/fuzzing\n";
    std::cout << "asan_runtime_safety_net: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section06/asan_runtime_safety_net", run>;

}  // namespace
