// Topic    : 栈展开与 RAII：异常路径自动释放资源
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 1.3
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section01_exception_mechanism
// Item     : stack_unwinding_with_raii
// Topic id : part2/stage09/section01/stack_unwinding_with_raii
// Refs     : https://en.cppreference.com/w/cpp/language/exceptions
//            https://en.cppreference.com/w/cpp/language/raii
//            ISO [except.ctor] [except.handle]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int g_dtor_log = 0;

struct Tracer {
    const char* name;
    explicit Tracer(const char* n) : name(n) { std::cout << "  +ctor " << name << '\n'; }
    ~Tracer() {
        ++g_dtor_log;
        std::cout << "  ~dtor " << name << '\n';
    }
    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;
};

std::mutex g_mtx;
int g_locked_depth = 0;

struct CountingLock {
    std::lock_guard<std::mutex> guard;
    explicit CountingLock(std::mutex& m) : guard(m) { ++g_locked_depth; }
    ~CountingLock() { --g_locked_depth; }
};

void deep_work(bool fail) {
    Tracer t_deep{"deep"};
    auto buf = std::make_unique<int[]>(64);  // RAII 堆内存
    buf[0] = 7;
    if (fail) {
        throw std::runtime_error("deep failure");
    }
    assert(buf[0] == 7);
}

void mid_work(bool fail) {
    Tracer t_mid{"mid"};
    CountingLock lock{g_mtx};  // RAII 锁
    deep_work(fail);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [stack_unwinding_with_raii] 主干：抛异常时逆序析构 ===\n";
    {
        g_dtor_log = 0;
        g_locked_depth = 0;
        [[maybe_unused]] bool caught = false;
        try {
            Tracer outer{"outer"};
            mid_work(/*fail=*/true);
            assert(false);
        } catch (const std::runtime_error& e) {
            caught = true;
            std::cout << "caught: " << e.what() << '\n';
        }
        assert(caught);
        // deep → mid → outer 逆序析构都应完成
        assert(g_dtor_log == 3);
        // 锁必须在栈展开中释放，否则后续会死锁
        assert(g_locked_depth == 0);
        std::cout << "dtors during unwind=" << g_dtor_log << " lock_depth=" << g_locked_depth << '\n';
    }

    std::cout << "=== 对抗：成功路径同样 RAII，无泄漏 ===\n";
    {
        g_dtor_log = 0;
        g_locked_depth = 0;
        mid_work(/*fail=*/false);
        assert(g_dtor_log == 2);  // deep + mid（无 outer）
        assert(g_locked_depth == 0);
        std::cout << "success path dtors=" << g_dtor_log << '\n';
    }

    std::cout << "=== 边界：容器/智能指针在异常路径的清理 ===\n";
    {
        std::vector<std::unique_ptr<Tracer>> items;
        items.push_back(std::make_unique<Tracer>("v0"));
        items.push_back(std::make_unique<Tracer>("v1"));
        try {
            items.push_back(std::make_unique<Tracer>("v2"));
            throw std::logic_error("abort batch");
        } catch (const std::logic_error& e) {
            std::cout << "batch aborted: " << e.what() << '\n';
        }
        // items 离开作用域 → unique_ptr 释放所有 Tracer
        items.clear();
        std::cout << "vector of unique_ptr cleaned on exception path\n";
    }

    std::cout << "=== 专节：栈展开中析构再抛 → terminate（只注释，不触发）===\n";
    // 若 ~Tracer 里再 throw，且当前已有异常在传播 → std::terminate。
    // 铁律：析构不得抛（见 section07）。本 topic 不故意 terminate。
    std::cout << "rule: destructor must not throw during unwinding\n";

    std::cout << "[stack_unwinding_with_raii] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section01/stack_unwinding_with_raii", run>;

}  // namespace
