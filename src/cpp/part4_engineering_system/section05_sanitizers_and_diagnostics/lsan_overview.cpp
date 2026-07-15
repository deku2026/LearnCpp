// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.1 LSan
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : lsan_overview
// Topic id : part4/section05/lsan_overview
//
// 要点: 泄漏检测; 常随 ASan 默认开启; RAII/智能指针是正解。
//       对照 cmake/Sanitizers.cmake: LEARNCPP_ENABLE_ASAN 附带泄漏检查能力。
//       本文件不故意泄漏(避免污染 CI)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 泄漏模式(反例注释): new 丢失指针 → 应靠 RAII 杜绝
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

void shared_cycle_note() {
    // shared_ptr 环可“逻辑泄漏”; LSan 不一定报(仍可达)
    // 工程上用 weak_ptr 破环 — 此处仅文档化
    std::cout << "  note: shared_ptr cycles may evade LSan (still reachable)\n";
}

// suppress 心智: LSAN_OPTIONS=suppressions=file
bool should_suppress(std::string_view frame) {
    return frame.find("third_party::") != std::string_view::npos;
}

// 与 ASan 关系(矩阵)
bool lsan_often_with_asan() {
    return true;
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
    assert(lsan_often_with_asan());

    // 独立 -fsanitize=leak 或 ASan 默认嵌 LSan
    const std::string with_asan = "-fsanitize=address";
    const std::string leak_only = "-fsanitize=leak";
    assert(with_asan.find("address") != std::string::npos);
    assert(leak_only.find("leak") != std::string::npos);
    std::cout << "  often bundled with ASan; standalone " << leak_only << '\n';
    std::cout << "  repo: LEARNCPP_ENABLE_ASAN in cmake/Sanitizers.cmake\n";

    shared_cycle_note();
    std::cout << "lsan_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/lsan_overview", run>;

}  // namespace
