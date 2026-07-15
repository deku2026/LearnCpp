// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.4 valgrind/memcheck
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : valgrind_overview
// Topic id : part4/section05/valgrind_overview
//
// 要点: 无需重编译插桩; 比 ASan 慢; 优先 ASan, valgrind 作补充(Linux)。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class Tool { Memcheck, Helgrind, Cachegrind, Callgrind };

const char* tool_name(Tool t) {
    switch (t) {
        case Tool::Memcheck:
            return "memcheck";
        case Tool::Helgrind:
            return "helgrind";
        case Tool::Cachegrind:
            return "cachegrind";
        case Tool::Callgrind:
            return "callgrind";
    }
    return "?";
}

// 错误类型 memcheck 关注
struct Issue {
    std::string kind;
    bool still_reachable;
};

// 模拟报告汇总
struct Report {
    std::vector<Issue> issues;
    int error_count() const {
        int n = 0;
        for (const auto& i : issues) {
            if (i.kind != "still_reachable" || !i.still_reachable) {
                // 简化: definitely lost 等算 error
            }
            if (i.kind == "definitely_lost" || i.kind == "invalid_read" || i.kind == "invalid_write") {
                ++n;
            }
        }
        return n;
    }
};

// 相对开销(教学数量级)
double slowdown_vs_native(Tool t) {
    switch (t) {
        case Tool::Memcheck:
            return 20.0;
        case Tool::Helgrind:
            return 50.0;
        case Tool::Cachegrind:
            return 15.0;
        case Tool::Callgrind:
            return 30.0;
    }
    return 1.0;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== valgrind_overview ===\n";

    assert(std::string_view(tool_name(Tool::Memcheck)) == "memcheck");
    assert(slowdown_vs_native(Tool::Memcheck) > 5.0);
    std::cout << "  memcheck slowdown ~x" << slowdown_vs_native(Tool::Memcheck) << '\n';

    Report clean{};
    assert(clean.error_count() == 0);

    Report leaky{{{"definitely_lost", false}, {"still_reachable", true}}};
    assert(leaky.error_count() == 1);
    std::cout << "  sample report errors=" << leaky.error_count() << '\n';

    // CLI: valgrind --leak-check=full ./app
    [[maybe_unused]] const char* cli = "valgrind --leak-check=full ./app";
    assert(std::string_view(cli).find("leak-check") != std::string_view::npos);

    // 对比 ASan: 需重编译但更快更准(边界毒化)
    [[maybe_unused]] const bool prefer_asan_in_ci = true;
    assert(prefer_asan_in_ci);
    std::cout << "  prefer ASan in CI; valgrind when cannot recompile\n";

    // Helgrind ≈ 线程问题(类 TSan); Cachegrind 缓存剖析
    std::cout << "  tools: " << tool_name(Tool::Helgrind) << ", " << tool_name(Tool::Cachegrind) << ", "
              << tool_name(Tool::Callgrind) << '\n';

    std::cout << "valgrind_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/valgrind_overview", run>;

}  // namespace
