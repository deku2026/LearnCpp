// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.4 覆盖率 gcov/llvm-cov
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : coverage_overview
// Topic id : part4/section05/coverage_overview
//
// 要点: --coverage / -fprofile-instr-generate; 看行/分支覆盖, 指导补测。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 插桩计数器模拟
struct CovMap {
    std::map<int, int> line_hits;  // line -> count
    void hit(int line) { ++line_hits[line]; }
};

int classify(int x, CovMap& cov) {
    cov.hit(1);  // entry
    if (x < 0) {
        cov.hit(2);
        return -1;
    }
    cov.hit(3);
    if (x == 0) {
        cov.hit(4);
        return 0;
    }
    cov.hit(5);
    return 1;
}

double line_coverage(const CovMap& cov, const std::vector<int>& executable_lines) {
    int covered = 0;
    for (int ln : executable_lines) {
        auto it = cov.line_hits.find(ln);
        if (it != cov.line_hits.end() && it->second > 0) {
            ++covered;
        }
    }
    return executable_lines.empty() ? 0.0 : static_cast<double>(covered) / static_cast<double>(executable_lines.size());
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== coverage_overview ===\n";

    CovMap cov;
    // 只测正数路径 → 覆盖不全
    assert(classify(5, cov) == 1);
    const std::vector<int> lines{1, 2, 3, 4, 5};
    double partial = line_coverage(cov, lines);
    assert(partial < 1.0);
    std::cout << "  after one path, line cov=" << partial << '\n';

    // 补测负与零
    assert(classify(-1, cov) == -1);
    assert(classify(0, cov) == 0);
    double full = line_coverage(cov, lines);
    assert(full == 1.0);
    std::cout << "  after all branches, line cov=" << full << '\n';

    // 工具旗标
    // GCC: --coverage  (等价 -fprofile-arcs -ftest-coverage)
    // Clang: -fprofile-instr-generate -fcoverage-mapping + llvm-cov
    const char* gcc_flag = "--coverage";
    const char* clang_prof = "-fprofile-instr-generate";
    assert(std::string_view(gcc_flag) == "--coverage");
    assert(std::string_view(clang_prof).find("profile") != std::string_view::npos);

    // 目标: 关键路径高覆盖, 非追求虚高 %
    assert(full >= 0.9);
    std::cout << "coverage_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/coverage_overview", run>;

}  // namespace
