// LearnCpp topic
// Doc      : part6-branch-b-lifetime-and-ownership.md (B1 存储期)
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : storage_duration_phases
// Topic id : part6/b/section01/storage_duration_phases
//
// 要点: 四种存储期决定分配/释放时机——automatic / static / thread / dynamic。
// 参考: [basic.stc]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace {

[[maybe_unused]] int g_static = 100;

thread_local int tls_counter = 0;

struct Tracker {
    const char* tag;
    explicit Tracker(const char* t) : tag(t) { std::cout << "  ctor " << tag << '\n'; }
    ~Tracker() { std::cout << "  dtor " << tag << '\n'; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== B1 storage duration phases ===\n";

    // automatic
    {
        Tracker auto_obj{"automatic"};
        assert(auto_obj.tag[0] == 'a');
    }

    // static
    assert(g_static == 100);
    {
        static Tracker once{"static-local"};
        assert(once.tag[0] == 's');
    }
    {
        // 再次进入：不重复构造
        static Tracker& again = []() -> Tracker& {
            static Tracker once{"static-local"};
            return once;
        }();
        (void)again;
    }

    // thread
    tls_counter = 7;
    int other = 0;
    std::thread t([&] {
        assert(tls_counter == 0);
        tls_counter = 99;
        other = tls_counter;
    });
    t.join();
    assert(tls_counter == 7);
    assert(other == 99);

    // dynamic
    {
        auto p = std::make_unique<Tracker>("dynamic");
        assert(p->tag[0] == 'd');
    }

    // 同一类型可放在任意存储期——由声明方式决定
    std::string stack_s = "stack";
    auto heap_s = std::make_unique<std::string>("heap");
    static std::string static_s = "static";
    assert(stack_s.size() == 5);
    assert(*heap_s == "heap");
    assert(static_s == "static");

    std::cout << "  automatic: scope / static: program / thread: thread / dynamic: new\n";
    std::cout << "  storage duration ≠ type; creation context decides\n";
    std::cout << "storage_duration_phases: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/storage_duration_phases", run>;

}  // namespace
