// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.1 LSan
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : lsan_overview
// Topic id : part4/section05/lsan_overview
//
// 要点: 泄漏检测; 常随 ASan 默认开启。RAII/智能指针是根治。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 泄漏模式(概念): new 后丢失指针 — 我们用 RAII 对照
struct Tracker {
    static int live;
    Tracker() { ++live; }
    ~Tracker() { --live; }
    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;
};
int Tracker::live = 0;

void leaky_style_avoided() {
    // bad: Tracker* t = new Tracker; // 忘记 delete → LSan
    auto t = std::make_unique<Tracker>();
    assert(Tracker::live == 1);
}  // 自动释放

void container_owns() {
    std::vector<std::unique_ptr<Tracker>> v;
    v.push_back(std::make_unique<Tracker>());
    v.push_back(std::make_unique<Tracker>());
    assert(Tracker::live == 2);
    v.clear();
    assert(Tracker::live == 0);
}

// suppress 概念: 第三方泄漏可 LSAN_OPTIONS=suppressions
bool should_suppress(std::string_view frame) {
    return frame.find("third_party::") != std::string_view::npos;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== lsan_overview ===\n";

    assert(Tracker::live == 0);
    leaky_style_avoided();
    assert(Tracker::live == 0);
    container_owns();
    assert(Tracker::live == 0);
    std::cout << "  RAII keeps live objects at 0 on scope exit\n";

    assert(should_suppress("third_party::opaque_init"));
    assert(!should_suppress("myapp::parse"));

    // ASan 默认嵌 LSan; 也可 -fsanitize=leak
    std::cout << "  often bundled with ASan; standalone -fsanitize=leak\n";
    std::cout << "lsan_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/lsan_overview", run>;

}  // namespace
