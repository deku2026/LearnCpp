// Topic    : std::spanstream（C++23，固定缓冲、无堆分配流）
// Doc      : 第3部分-标准库系统化.md · 库 5.2
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : spanstream_cpp23
// Topic id : part3/section05/spanstream_cpp23
// Refs     : https://en.cppreference.com/w/cpp/io/basic_spanstream
//            P0448；__cpp_lib_spanstream

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<spanstream>)
#include <spanstream>
#define LEARN_HAS_SPANSTREAM 1
#endif
#endif
#ifndef LEARN_HAS_SPANSTREAM
#define LEARN_HAS_SPANSTREAM 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [spanstream_cpp23] ===\n";

#if LEARN_HAS_SPANSTREAM && defined(__cpp_lib_spanstream)
    std::cout << "__cpp_lib_spanstream=" << __cpp_lib_spanstream << '\n';

    // ospanstream：写到固定 char 缓冲，不 new
    {
        char buf[64]{};
        std::ospanstream oss{std::span<char>{buf}};
        oss << "x=" << 42 << " ok";
        assert(oss);
        // 有效输出区：span() 返回已写范围（实现相关 API：.span()）
        const auto written = oss.span();
        const std::string_view sv{written.data(), written.size()};
        assert(sv == "x=42 ok");
        std::cout << "ospanstream: [" << sv << "] bytes=" << written.size() << '\n';
    }

    // ispanstream：从既有缓冲解析
    {
        const char data[] = "7 11 13";
        std::ispanstream iss{std::span<const char>{data, sizeof(data) - 1}};
        int a = 0, b = 0, c = 0;
        iss >> a >> b >> c;
        assert(a == 7 && b == 11 && c == 13);
        std::cout << "ispanstream sum=" << (a + b + c) << '\n';
    }

    // spanstream 双向
    {
        char buf[32]{};
        std::spanstream ss{std::span<char>{buf}};
        ss << 100;
        ss.seekg(0);
        int n = 0;
        ss >> n;
        assert(n == 100);
        std::cout << "spanstream round-trip n=" << n << '\n';
    }

    std::cout << "use case: stack buffer formatting without heap churn\n";
#else
    std::cout << "<spanstream> / __cpp_lib_spanstream not available; "
                 "fallback: fixed buffer + snprintf style note\n";
    char buf[64]{};
    // 探测：固定缓冲仍可手动格式化
#if defined(_MSC_VER)
    _snprintf_s(buf, sizeof(buf), _TRUNCATE, "x=%d ok", 42);
#else
    std::snprintf(buf, sizeof(buf), "x=%d ok", 42);
#endif
    assert(std::string_view{buf} == "x=42 ok");
    std::cout << "manual fixed buf: " << buf << '\n';
#endif

    std::cout << "[spanstream_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/spanstream_cpp23", run>;

}  // namespace
