// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.6.3 跨平台验证价值
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : cross_platform_verification
// Topic id : part4/section06/cross_platform_verification
//
// 要点: 端序、long 宽度、路径分隔、对齐在跨平台才暴露假设错误。

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

namespace {

bool is_little_endian() {
    const std::uint16_t x = 0x0102;
    const auto* p = reinterpret_cast<const unsigned char*>(&x);
    return p[0] == 0x02;
}

char preferred_path_sep() {
#if defined(_WIN32)
    return '\\';
#else
    return '/';
#endif
}

// 可移植路径拼接: 统一用 /, 多数 Win API 也接受
std::string join_path(std::string_view a, std::string_view b) {
    std::string out(a);
    if (!out.empty() && out.back() != '/' && out.back() != '\\') {
        out.push_back('/');
    }
    out.append(b);
    return out;
}

struct PlatformSnapshot {
    const char* os;
    bool little_endian;
    int long_bits;
    int ptr_bits;
    char path_sep;
};

PlatformSnapshot snapshot() {
    PlatformSnapshot s{};
#if defined(_WIN32)
    s.os = "Windows";
#elif defined(__APPLE__)
    s.os = "macOS";
#elif defined(__linux__)
    s.os = "Linux";
#else
    s.os = "Other";
#endif
    s.little_endian = is_little_endian();
    s.long_bits = static_cast<int>(sizeof(long) * 8);
    s.ptr_bits = static_cast<int>(sizeof(void*) * 8);
    s.path_sep = preferred_path_sep();
    return s;
}

// 错误假设: long 总是 64 位(LLP64 Windows 上 long=32)
bool mistaken_long_is_64() {
    return sizeof(long) == 8;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== cross_platform_verification ===\n";

    auto s = snapshot();
    std::cout << "  os=" << s.os << " endian=" << (s.little_endian ? "LE" : "BE") << " long=" << s.long_bits << "bit"
              << " ptr=" << s.ptr_bits << "bit"
              << " path_sep='" << s.path_sep << "'\n";

    assert(s.ptr_bits == 32 || s.ptr_bits == 64);
    // 桌面主流 LE
    assert(s.little_endian);

    auto path = join_path("src/cpp", "main.cpp");
    assert(path.find("main.cpp") != std::string::npos);
    assert(path.find('/') != std::string::npos);
    std::cout << "  portable join: " << path << '\n';

    // Windows LLP64: long=32, 指针=64 — 矩阵能抓住「用 long 存指针」类 bug
    if (std::string_view(s.os) == "Windows") {
        assert(sizeof(long) == 4);
        assert(sizeof(void*) == 8 || sizeof(void*) == 4);
        std::cout << "  Windows LLP64: do not store pointers in long\n";
    } else {
        // LP64: long 常 64
        std::cout << "  non-Windows: long_bits=" << s.long_bits << '\n';
    }
    (void)mistaken_long_is_64;

    // 固定宽度类型
    assert(sizeof(std::int32_t) == 4);
    assert(sizeof(std::int64_t) == 8);
    std::cout << "  prefer int32_t/int64_t for wire formats\n";

    std::cout << "cross_platform_verification: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section06/cross_platform_verification", run>;

}  // namespace
