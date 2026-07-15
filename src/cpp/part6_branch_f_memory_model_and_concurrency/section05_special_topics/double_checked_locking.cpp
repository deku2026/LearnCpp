// LearnCpp topic
// Doc      : 第6部分-支线F · F9.1 DCL
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : double_checked_locking
// Topic id : part6/f/section05/double_checked_locking
//
// 要点: 正确 DCL 需要原子 + acquire/release; 裸指针+mutex 旧写法在 C++ 内存模型下是错的。
// 参考: Meyers/Alexandrescu DCL; [threads.mutex]

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace {

class Singleton {
public:
    static Singleton& instance() {
        Singleton* p = instance_.load(std::memory_order_acquire);
        if (!p) {
            std::lock_guard<std::mutex> lock(mtx_);
            p = instance_.load(std::memory_order_relaxed);
            if (!p) {
                p = new Singleton();
                instance_.store(p, std::memory_order_release);
            }
        }
        return *p;
    }

    int value() const { return value_; }

    // 测试用重置 (非线程安全; 仅单测)
    static void reset_for_test() { delete instance_.exchange(nullptr); }

private:
    Singleton() : value_(42) {}
    int value_;
    static std::atomic<Singleton*> instance_;
    static std::mutex mtx_;
};

std::atomic<Singleton*> Singleton::instance_{nullptr};
std::mutex Singleton::mtx_;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F9 double-checked locking ===\n";

    Singleton::reset_for_test();

    std::vector<std::jthread> ts;
    std::atomic<int> sum{0};
    for (int i = 0; i < 8; ++i) {
        ts.emplace_back([&] { sum.fetch_add(Singleton::instance().value(), std::memory_order_relaxed); });
    }
    ts.clear();
    assert(sum.load() == 8 * 42);
    assert(Singleton::instance().value() == 42);

    std::cout << "  first null-check without lock; second under mutex\n";
    std::cout << "  publish with release; observe with acquire\n";
    std::cout << "  prefer function-local static or call_once in real code\n";

    Singleton::reset_for_test();
    std::cout << "double_checked_locking: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/double_checked_locking", run>;

}  // namespace
