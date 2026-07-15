// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.1 三编译器矩阵
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : compiler_matrix_gcc_clang_msvc
// Topic id : part4/section06/compiler_matrix_gcc_clang_msvc
//
// 要点: GCC/Clang/MSVC 对 C++23 落地节奏不同; 矩阵暴露实现差。
// 本机探测当前编译器宏。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class CompilerId { Gcc, Clang, Msvc, ClangCl, Unknown };

CompilerId detect() {
#if defined(__clang__) && defined(_MSC_VER)
    return CompilerId::ClangCl;
#elif defined(__clang__)
    return CompilerId::Clang;
#elif defined(_MSC_VER)
    return CompilerId::Msvc;
#elif defined(__GNUC__)
    return CompilerId::Gcc;
#else
    return CompilerId::Unknown;
#endif
}

const char* cname(CompilerId id) {
    switch (id) {
        case CompilerId::Gcc:
            return "gcc";
        case CompilerId::Clang:
            return "clang";
        case CompilerId::Msvc:
            return "msvc";
        case CompilerId::ClangCl:
            return "clang-cl";
        case CompilerId::Unknown:
            return "unknown";
    }
    return "?";
}

struct FeatureRow {
    const char* name;
    long macro_value;  // 0 = missing
    bool required_in_ci;
};

long probe_print() {
#if defined(__cpp_lib_print)
    return __cpp_lib_print;
#else
    return 0;
#endif
}

long probe_expected() {
#if defined(__cpp_lib_expected)
    return __cpp_lib_expected;
#else
    return 0;
#endif
}

long probe_stacktrace() {
#if defined(__cpp_lib_stacktrace)
    return __cpp_lib_stacktrace;
#else
    return 0;
#endif
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== compiler_matrix_gcc_clang_msvc ===\n";

    const auto id = detect();
    std::cout << "  this binary built with: " << cname(id) << '\n';
#if defined(__clang_major__)
    std::cout << "  clang major=" << __clang_major__ << '\n';
#endif
#if defined(_MSC_VER)
    std::cout << "  _MSC_VER=" << _MSC_VER << '\n';
#endif
#if defined(__GNUC__) && !defined(__clang__)
    std::cout << "  gcc " << __GNUC__ << "." << __GNUC_MINOR__ << '\n';
#endif

    // 本课程基线工具链应能识别
    assert(id == CompilerId::ClangCl || id == CompilerId::Clang || id == CompilerId::Msvc || id == CompilerId::Gcc);

    // CI 应覆盖的三前端(clang-cl 归 MSVC ABI 车道)
    std::vector<std::string> ci_compilers{"gcc", "clang", "msvc"};
    assert(ci_compilers.size() == 3);
    assert(ci_compilers[0] == "gcc" && ci_compilers[2] == "msvc");

    // 映射: 当前 id 落在哪条 CI 车道
    auto lane_for = [](CompilerId c) -> std::string_view {
        switch (c) {
            case CompilerId::Gcc:
                return "gcc";
            case CompilerId::Clang:
                return "clang";
            case CompilerId::Msvc:
            case CompilerId::ClangCl:
                return "msvc";
            default:
                return "unknown";
        }
    };
    assert(lane_for(id) != "unknown");
    std::cout << "  CI lane: " << lane_for(id) << '\n';

    FeatureRow rows[] = {
        {"__cpp_lib_print", probe_print(), false},
        {"__cpp_lib_expected", probe_expected(), false},
        {"__cpp_lib_stacktrace", probe_stacktrace(), false},
    };
    [[maybe_unused]] int present = 0;
    for (const auto& r : rows) {
        std::cout << "  " << r.name << "=" << r.macro_value << (r.macro_value ? " (present)" : " (missing)") << '\n';
        if (r.macro_value != 0) {
            ++present;
        }
    }
    // 不强制 C++23 全开, 但矩阵输出必须完整
    assert(present >= 0);
    assert(sizeof(rows) / sizeof(rows[0]) == 3);

    // 仓库基线: VS2026 clang-cl + MSVC STL; CMakePresets windows-* 使用 clang-cl
#if defined(__clang__) && defined(_MSC_VER)
    assert(id == CompilerId::ClangCl);
#endif
    std::cout << "  baseline doc: VS clang-cl + MSVC STL; CI adds GCC/Clang\n";
    std::cout << "compiler_matrix_gcc_clang_msvc: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/compiler_matrix_gcc_clang_msvc", run>;

}  // namespace
