// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B1 存储期)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : storage_duration_phases
// Topic id : part6/b/section01/storage_duration_phases
//
// 要点: 四种存储期决定分配/释放时机——automatic / static / thread / dynamic。
// 参考: [basic.stc] https://en.cppreference.com/w/cpp/language/storage_duration

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace {

int g_static = 100;  // 静态存储期：程序启动前初始化，结束时销毁

thread_local int tls_counter = 0;  // 线程存储期：每线程一份

struct Tracker {
    const char* tag;
    explicit Tracker(const char* t) : tag(t) { std::cout << "  ctor " << tag << '\n'; }
    ~Tracker() { std::cout << "  dtor " << tag << '\n'; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B1 storage duration phases ===\n";

    // --- automatic：进入作用域分配，离开释放 ---
    {
        Tracker auto_obj{"automatic"};
        assert(auto_obj.tag[0] == 'a');
    }  // 此处析构

    // --- static：全局 / 函数局部 static ---
    assert(g_static == 100);
    {
        static Tracker once{"static-local"};  // 首次进入时构造，程序结束析构
        assert(once.tag[0] == 's');
    }
    {
        static Tracker& again = []() -> Tracker& {
            static Tracker once{"static-local"};
            return once;
        }();
        // 同一函数局部 static 只构造一次（上面块已构造）
        (void)again;
    }

    // --- thread：每线程独立 ---
    tls_counter = 7;
    int other = 0;
    std::thread t([&] {
        assert(tls_counter == 0);  // 新线程从 0 开始
        tls_counter = 99;
        other = tls_counter;
    });
    t.join();
    assert(tls_counter == 7);  // 主线程未变
    assert(other == 99);

    // --- dynamic：new/delete 或智能指针显式控制 ---
    {
        auto p = std::make_unique<Tracker>("dynamic");
        assert(p->tag[0] == 'd');
    }  // unique_ptr 析构 → delete

    // 同一类型可放在任意存储期——由声明方式决定，不是类型本身
    std::string stack_s = "stack";
    auto heap_s = std::make_unique<std::string>("heap");
    static std::string static_s = "static";
    assert(stack_s.size() == 5);
    assert(*heap_s == "heap");
    assert(static_s == "static");

    std::cout << "storage_duration_phases: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/storage_duration_phases", run>;

}  // namespace
