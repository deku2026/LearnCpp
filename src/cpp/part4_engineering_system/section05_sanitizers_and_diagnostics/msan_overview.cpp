// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.1 MSan
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : msan_overview
// Topic id : part4/section05/msan_overview
//
// 要点: 抓读未初始化内存; 仅 Clang; 需全依赖(含 libc++) MSan 插桩否则误报。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {

bool msan_built_in() {
#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
    return true;
#endif
#endif
    return false;
}

// 错误模式: int x; use(x) — 我们演示初始化纪律
int sum_defined(const std::vector<int>& v) {
    int s = 0;  // 必须初始化
    for (int x : v) {
        s += x;
    }
    return s;
}

// 可选缓冲: 只读已写入前缀
struct Buf {
    char data[16]{};
    std::size_t len = 0;
    void append(char c) {
        if (len < sizeof(data)) {
            data[len++] = c;
        }
    }
    std::string as_string() const { return std::string(data, len); }
};

// MSan 门槛检查表
struct MsanPrereq {
    bool clang;
    bool instrumented_stdlib;
    bool instrumented_deps;
    bool ok() const { return clang && instrumented_stdlib && instrumented_deps; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== msan_overview ===\n";
    std::cout << "  msan: " << (msan_built_in() ? "ON" : "OFF") << '\n';

    assert(sum_defined({1, 2, 3}) == 6);

    Buf b;
    b.append('h');
    b.append('i');
    assert(b.as_string() == "hi");
    // 不读 data[2..] 未定义内容
    std::cout << "  only read initialized prefix of buffers\n";

    MsanPrereq hard{true, false, false};
    assert(!hard.ok());
    MsanPrereq full{true, true, true};
    assert(full.ok());
    std::cout << "  MSan needs fully instrumented stack (stdlib+deps)\n";

    // 旗标: -fsanitize=memory
    std::cout << "  typically Linux Clang-only specialized lane\n";
    std::cout << "msan_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/msan_overview", run>;

}  // namespace
