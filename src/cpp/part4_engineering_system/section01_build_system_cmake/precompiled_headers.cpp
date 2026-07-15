// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.5 PCH 预编译头
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : precompiled_headers
// Topic id : part4/section01/precompiled_headers
//
// 要点: target_precompile_headers 把稳定重型头预解析一次, 各 TU 复用。
//       适合 <vector>/<string>/<algorithm> 等; 频繁改的业务头不宜进 PCH。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 教学成本模型: 解析时间近似「头文件权重」
struct HeaderCost {
    std::string name;
    int parse_units;  // 相对代价
};

int cost_without_pch(const std::vector<HeaderCost>& common, int translation_units) {
    int per_tu = 0;
    for (const auto& h : common) {
        per_tu += h.parse_units;
    }
    return per_tu * translation_units;
}

int cost_with_pch(const std::vector<HeaderCost>& common, int translation_units, int pch_build_extra) {
    int per_tu_reuse = 1;  // 加载 PCH 的固定小代价
    int sum = 0;
    for (const auto& h : common) {
        sum += h.parse_units;
    }
    // 建一次 PCH + 每 TU 廉价复用
    return sum + pch_build_extra + per_tu_reuse * translation_units;
}

// 哪些头适合进 PCH
[[maybe_unused]] bool stable_enough_for_pch(std::string_view path, int churn_per_week) {
    if (path.find("third_party/") != std::string_view::npos) {
        return churn_per_week == 0;
    }
    if (path.starts_with("<") && path.ends_with(">")) {
        return true;  // 标准库头
    }
    return churn_per_week <= 1;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== precompiled_headers ===\n";

    const std::vector<HeaderCost> heavy{
        {"<vector>", 40},
        {"<string>", 35},
        {"<algorithm>", 50},
        {"<unordered_map>", 45},
    };
    constexpr int tus = 50;
    const int no_pch = cost_without_pch(heavy, tus);
    const int yes_pch = cost_with_pch(heavy, tus, /*pch_build_extra=*/20);
    assert(yes_pch < no_pch);
    std::cout << "  model cost no_pch=" << no_pch << " with_pch=" << yes_pch << " speedup~x"
              << (static_cast<double>(no_pch) / yes_pch) << '\n';

    assert(stable_enough_for_pch("<vector>", 0));
    assert(stable_enough_for_pch("third_party/fmt/format.h", 0));
    assert(!stable_enough_for_pch("src/core/engine.hpp", 12));

    // CMake 写法(文档):
    // target_precompile_headers(myapp PRIVATE <vector> <string>)
    // REUSE_FROM 可让 test target 复用 app 的 PCH
    [[maybe_unused]] const bool cmake_api = true;
    assert(cmake_api);
    std::cout << "  CMake: target_precompile_headers(tgt PRIVATE <vector>...)\n";

    // 专家: PCH 与 sccache — 缓存键含 PCH 输入; 改 PCH 列表会大面积失效
    [[maybe_unused]] int cache_invalidations_when_pch_list_changes = tus;  // 所有依赖 TU
    assert(cache_invalidations_when_pch_list_changes == tus);
    std::cout << "  note: changing PCH inputs invalidates ~" << tus << " TUs\n";

    // 运行时「已包含」标准库 — 编译期真实可用(本文件即受益于常规包含)
    std::vector<std::string> demo{"pch", "saves", "parse"};
    assert(demo.size() == 3);

    std::cout << "precompiled_headers: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/precompiled_headers", run>;

}  // namespace
