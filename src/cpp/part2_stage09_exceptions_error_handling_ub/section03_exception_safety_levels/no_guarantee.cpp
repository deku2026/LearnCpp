// Topic    : No guarantee：抛后可能泄漏/损坏（应极力避免）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 3.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : no_guarantee
// Topic id : part2/stage09/section03/no_guarantee
// Refs     : https://en.cppreference.com/w/cpp/language/exceptions
//            Exceptional C++ (Sutter)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace {

// 反面教材：裸 new + 中途可能抛，且没有 RAII → 潜在泄漏（no guarantee）
// 注意：本 demo 用“模拟”展示问题结构，最终仍安全清理，避免真泄漏拖垮测试。
struct Nasty {
    int* raw = nullptr;
    std::string tag;

    void bad_update(const std::string& t, bool fail) {
        // 1) 分配成功
        int* fresh = new int[8]{};
        // 2) 可能抛的操作 —— 若这里抛，fresh 泄漏（no guarantee 典型）
        std::string tmp = t;
        if (fail) {
            // 教学：若直接 throw，fresh 泄漏。
            // 为保持 topic 可重复安全运行，这里演示“错误结构”后立刻补救。
            delete[] fresh;  // 补救：真实坏代码不会写这行
            throw std::runtime_error("update failed after raw new");
        }
        // 3) 提交
        delete[] raw;
        raw = fresh;
        tag = std::move(tmp);
    }

    // 正确对照：RAII
    void good_update(const std::string& t, bool fail) {
        auto fresh = std::make_unique<int[]>(8);
        std::string tmp = t;
        if (fail) {
            throw std::runtime_error("update failed (RAII path)");
        }
        // 提交
        raw = fresh.release();  // 简化：真实类型应全程 unique_ptr 成员
        tag = std::move(tmp);
    }

    ~Nasty() { delete[] raw; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [no_guarantee] 主干：反面结构（裸资源 + 中途抛）===\n";
    {
        Nasty n;
        try {
            n.bad_update("x", /*fail=*/true);
            assert(false);
        } catch (const std::runtime_error& e) {
            std::cout << "bad_update path: " << e.what() << '\n';
        }
        // 注释中的“泄漏结构”：new 成功 → 后续 throw → 无主指针。
        // 本文件在 throw 前 delete 仅为教学进程安全，不代表原逻辑安全。
        std::cout << "anti-pattern: raw new then throw without owner\n";
    }

    std::cout << "=== 对抗：RAII 把 no-guarantee 抬到至少 basic ===\n";
    {
        Nasty n;
        try {
            // 这里用 make_unique 版本；失败时 fresh 自动释放
            n.good_update("ok", /*fail=*/true);
            assert(false);
        } catch (const std::runtime_error& e) {
            std::cout << "good_update path: " << e.what() << '\n';
        }
        assert(n.raw == nullptr);
        std::cout << "RAII path: no leak, object still destructible\n";
    }

    std::cout << "=== 专节：纪律 ===\n";
    // 任何“获取资源 → 可能抛 → 释放资源”若不用 RAII/scope_guard，
    // 就容易掉进 no guarantee。现代 C++ 默认目标：至少 basic。
    std::cout << "avoid no-guarantee: own resources with RAII always\n";

    std::cout << "[no_guarantee] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/no_guarantee", run>;

}  // namespace
