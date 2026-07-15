// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.5 ccache / 编译缓存
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : ccache_versus_sccache
// Topic id : part4/section01/ccache_versus_sccache
//
// 要点: COMPILER_LAUNCHER 拦截编译, 相同输入哈希命中则跳过真正编译。
//       ccache 偏 Unix; sccache 跨平台 + 可选云缓存(本仓库默认 sccache)。
// 仓库: cmake/Sccache.cmake LEARNCPP_USE_SCCACHE; base preset 默认 ON

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace {

// 极简内容哈希(教学, 非加密)
std::uint64_t hash_key(std::string_view compiler, std::string_view flags, std::string_view source) {
    std::uint64_t h = 1469598103934665603ull;
    auto mix = [&](std::string_view s) {
        for (unsigned char c : s) {
            h ^= c;
            h *= 1099511628211ull;
        }
        h ^= 0xFF;
        h *= 1099511628211ull;
    };
    mix(compiler);
    mix(flags);
    mix(source);
    return h;
}

struct Cache {
    std::unordered_map<std::uint64_t, std::string> store;  // key -> object token
    int hits = 0;
    int misses = 0;

    std::string compile(std::string_view compiler, std::string_view flags, std::string_view source,
                        std::string_view obj_token) {
        const auto k = hash_key(compiler, flags, source);
        if (auto it = store.find(k); it != store.end()) {
            ++hits;
            return it->second;
        }
        ++misses;
        store.emplace(k, std::string(obj_token));
        return std::string(obj_token);
    }
};

enum class Launcher { None, Ccache, Sccache };

// 本仓库策略: Windows/跨平台优先 sccache
Launcher prefer_for(std::string_view os) {
    if (os == "Windows") {
        return Launcher::Sccache;
    }
    // Linux/macOS 两者皆可; 与仓库一致选 sccache
    return Launcher::Sccache;
}

const char* name(Launcher l) {
    switch (l) {
        case Launcher::None:
            return "none";
        case Launcher::Ccache:
            return "ccache";
        case Launcher::Sccache:
            return "sccache";
    }
    return "?";
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== ccache_versus_sccache ===\n";

    // --- 入门: 命中/未命中 ---
    Cache cache;
    auto o1 = cache.compile("clang-cl", "/std:c++latest /O0", "a.cpp", "a.obj#1");
    auto o2 = cache.compile("clang-cl", "/std:c++latest /O0", "a.cpp", "a.obj#2");
    assert(cache.misses == 1);
    assert(cache.hits == 1);
    assert(o1 == o2);  // 命中复用第一次产物
    std::cout << "  same input: hits=" << cache.hits << " misses=" << cache.misses << '\n';

    // 改 flags → 未命中
    cache.compile("clang-cl", "/std:c++latest /O2", "a.cpp", "a.obj#3");
    assert(cache.misses == 2);
    std::cout << "  flags change -> miss, total misses=" << cache.misses << '\n';

    // --- 进阶: launcher 选择 ---
    assert(prefer_for("Windows") == Launcher::Sccache);
    assert(prefer_for("Linux") == Launcher::Sccache);
    std::cout << "  LearnCpp prefer launcher=" << name(prefer_for("Windows")) << '\n';

    // --- 专家: 对照 Sccache.cmake ---
    // find_program(sccache); set CMAKE_CXX_COMPILER_LAUNCHER
    // MSVC/clang-cl: CMAKE_MSVC_DEBUG_INFORMATION_FORMAT=Embedded (缓存友好)
    const bool embedded_debug_for_msvc = true;  // presets + Sccache.cmake
    assert(embedded_debug_for_msvc);

    // ccache vs sccache 特性表(可运行探测用常量)
    struct Row {
        const char* tool;
        bool windows_first_class;
        bool remote_cache;
    };
    const Row table[] = {
        {"ccache", false, true},  // 有 remote 但 Windows 体验一般
        {"sccache", true, true},
    };
    assert(table[1].windows_first_class);
    std::cout << "  sccache: windows_first_class + remote_cache\n";

    // 统计比: 二次全量应高命中
    Cache warm;
    for (int i = 0; i < 5; ++i) {
        warm.compile("clang++", "-std=c++23", "t.cpp", "t.o");
    }
    assert(warm.hits == 4 && warm.misses == 1);
    const double rate = static_cast<double>(warm.hits) / static_cast<double>(warm.hits + warm.misses);
    assert(rate >= 0.8);
    std::cout << "  warm hit rate=" << rate << '\n';

    std::cout << "ccache_versus_sccache: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/ccache_versus_sccache", run>;

}  // namespace
