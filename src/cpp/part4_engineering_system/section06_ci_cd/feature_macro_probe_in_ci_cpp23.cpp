// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.1/4.6.3 __cpp_lib_* 特性探测
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : feature_macro_probe_in_ci_cpp23
// Topic id : part4/section06/feature_macro_probe_in_ci_cpp23
//
// 要点: CI 不靠「编译器版本应该够了」; 用 feature-test 宏条件编译/报告。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <version>

namespace {

struct Probe {
    const char* name;
    long value;  // 0 if absent
};

long v_print() {
#if defined(__cpp_lib_print)
    return static_cast<long>(__cpp_lib_print);
#else
    return 0;
#endif
}
long v_expected() {
#if defined(__cpp_lib_expected)
    return static_cast<long>(__cpp_lib_expected);
#else
    return 0;
#endif
}
long v_ranges() {
#if defined(__cpp_lib_ranges)
    return static_cast<long>(__cpp_lib_ranges);
#else
    return 0;
#endif
}
long v_format() {
#if defined(__cpp_lib_format)
    return static_cast<long>(__cpp_lib_format);
#else
    return 0;
#endif
}
long v_span() {
#if defined(__cpp_lib_span)
    return static_cast<long>(__cpp_lib_span);
#else
    return 0;
#endif
}
long v_generator() {
#if defined(__cpp_lib_generator)
    return static_cast<long>(__cpp_lib_generator);
#else
    return 0;
#endif
}
long v_flat_map() {
#if defined(__cpp_lib_flat_map)
    return static_cast<long>(__cpp_lib_flat_map);
#else
    return 0;
#endif
}

// CI 门禁: 关键特性缺失则失败(示例策略)
bool gate_cpp20_baseline(const std::vector<Probe>& probes) {
    for (const auto& p : probes) {
        if (std::string_view(p.name) == "__cpp_lib_span" && p.value == 0) {
            return false;
        }
        if (std::string_view(p.name) == "__cpp_lib_ranges" && p.value == 0) {
            return false;
        }
    }
    return true;
}

// 可移植输出: 有 print 用 print, 否则 iostream(此处统一 iostream)
void portable_hello() {
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    // std::print("hi\n"); — 可能仍缺, 保守用 cout
    std::cout << "  path: could use std::print when fully available\n";
#else
    std::cout << "  path: iostream fallback\n";
#endif
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== feature_macro_probe_in_ci_cpp23 ===\n";

    std::vector<Probe> probes{
        {"__cpp_lib_span", v_span()},         {"__cpp_lib_ranges", v_ranges()},
        {"__cpp_lib_format", v_format()},     {"__cpp_lib_print", v_print()},
        {"__cpp_lib_expected", v_expected()}, {"__cpp_lib_generator", v_generator()},
        {"__cpp_lib_flat_map", v_flat_map()},
    };

    int present = 0;
    for (const auto& p : probes) {
        std::cout << "  " << p.name << " = " << p.value << '\n';
        if (p.value != 0) {
            ++present;
        }
    }
    assert(present >= 1);  // 至少应有部分 C++20 库宏

    // span/ranges 在 C++20 工具链应在
    if (v_span() != 0 && v_ranges() != 0) {
        assert(gate_cpp20_baseline(probes));
    }

    portable_hello();
    std::cout << "  CI tip: print matrix of macros per compiler job\n";
    std::cout << "feature_macro_probe_in_ci_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/feature_macro_probe_in_ci_cpp23", run>;

}  // namespace
