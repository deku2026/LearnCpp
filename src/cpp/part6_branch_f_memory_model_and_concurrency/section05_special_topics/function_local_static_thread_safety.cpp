// LearnCpp topic
// Doc      : 第6部分-支线F · F9.3 函数局部静态
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : function_local_static_thread_safety
// Topic id : part6/f/section05/function_local_static_thread_safety
//
// 要点: C++11 起函数内 static 初始化由实现保证线程安全 (magic static objects)。
// 参考: [stmt.dcl] p4

#include "learn/topic_registry.hpp"

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

namespace {

std::atomic<int> ctor_count{0};

struct Heavy {
    int id;
    Heavy() : id(++ctor_count) {}
};

Heavy& get_heavy() {
    static Heavy h;  // 并发首次进入: 只构造一次
    return h;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== F9 function-local static thread safety ===\n";

    ctor_count.store(0);
    std::vector<std::jthread> ts;
    std::atomic<int> ids_ok{0};
    for (int i = 0; i < 16; ++i) {
        ts.emplace_back([&] {
            Heavy& h = get_heavy();
            if (h.id == 1) {
                ids_ok.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    ts.clear();

    assert(ctor_count.load() == 1);
    assert(ids_ok.load() == 16);
    assert(get_heavy().id == 1);

    std::cout << "  Meyers singleton: static local is the simplest safe pattern\n";
    std::cout << "  note: destruction order at exit still subtle in plugins\n";
    std::cout << "function_local_static_thread_safety: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/function_local_static_thread_safety", run>;

}  // namespace
