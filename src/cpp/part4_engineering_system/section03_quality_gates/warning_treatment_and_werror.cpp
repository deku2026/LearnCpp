// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.3.4 警告治理 / -Werror
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : warning_treatment_and_werror
// Topic id : part4/section03/warning_treatment_and_werror
//
// 要点: -Wall -Wextra -Wpedantic -Werror (MSVC /W4 /WX); 不留警告债。
// 仓库: cmake/CompilerWarnings.cmake + LEARNCPP_WERROR; windows-ci/linux-ci ON

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

// 演示「曾触发警告」的正确修法

// -Wshadow
int outer = 1;
[[maybe_unused]] int shadow_fixed(int x) {
    return x + outer;  // 不重用 outer 作局部名
}

// -Wconversion: 显式缩小
[[maybe_unused]] std::uint8_t narrow_to_byte(int v) {
    assert(v >= 0 && v <= 255);
    return static_cast<std::uint8_t>(v);
}

// -Wuninitialized 避免: 先初始化
[[maybe_unused]] int sum_init(const std::vector<int>& v) {
    int s = 0;
    for (int x : v) {
        s += x;
    }
    return s;
}

// 未使用参数: 标记
void api(int /*reserved*/, int used) {
    assert(used >= 0);
}

struct WarningPolicy {
    bool wall;
    bool wextra;
    bool wpedantic;
    bool werror;
    int msvc_level;  // 4 == /W4
};

int policy_score(const WarningPolicy& p) {
    int s = 0;
    if (p.wall) {
        s += 1;
    }
    if (p.wextra) {
        s += 1;
    }
    if (p.wpedantic) {
        s += 1;
    }
    if (p.werror) {
        s += 10;
    }
    if (p.msvc_level >= 4) {
        s += 1;
    }
    return s;
}

// 第三方头: SYSTEM include 不报警告(概念)
[[maybe_unused]] bool is_system_include_path(std::string_view path) {
    return path.find("/usr/include") != std::string_view::npos ||
           path.find("vcpkg_installed") != std::string_view::npos || path.find("third_party") != std::string_view::npos;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== warning_treatment_and_werror ===\n";

    assert(shadow_fixed(2) == 3);
    assert(narrow_to_byte(200) == 200);
    assert(sum_init({1, 2, 3}) == 6);
    api(0, 1);

    // 仓库默认开发: WERROR OFF; CI preset ON
    WarningPolicy local{true, true, true, false, 4};
    WarningPolicy ci{true, true, true, true, 4};
    assert(policy_score(ci) > policy_score(local));
    std::cout << "  local score=" << policy_score(local) << " ci score=" << policy_score(ci) << " (Werror weight)\n";

    // CompilerWarnings.cmake: MSVC /W4; GNU/Clang -Wall -Wextra -Wpedantic -Wshadow
    assert(is_system_include_path("D:/vcpkg_installed/x64-windows/include"));
    assert(!is_system_include_path("src/cpp/main.cpp"));
    std::cout << "  isolate third-party via SYSTEM includes\n";

    // 警告即信号: 隐式转换、遮蔽、未初始化
    std::cout << "  treat warnings as bugs-in-waiting; CI LEARNCPP_WERROR=ON\n";

    std::cout << "warning_treatment_and_werror: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/warning_treatment_and_werror", run>;

}  // namespace
