// Topic    : C++23 spanstream — 以 span 为缓冲的无堆分配流
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 16.2
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : spanstream_cpp23
// Topic id : part2/stage10/section11/spanstream_cpp23
// Refs     : https://en.cppreference.com/w/cpp/header/spanstream
//            https://en.cppreference.com/w/cpp/io/basic_spanstream
//            P0448R4  Feature-test: __cpp_lib_spanstream

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <span>
#include <string>
#include <vector>

#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
#include <spanstream>
#define LEARN_HAS_SPANSTREAM 1
#else
// 部分 MSVC/libstdc++ 版本宏名或值差异: 尝试头文件探测
#if defined(__has_include)
#if __has_include(<spanstream>)
#include <spanstream>
#define LEARN_HAS_SPANSTREAM 1
#else
#define LEARN_HAS_SPANSTREAM 0
#endif
#else
#define LEARN_HAS_SPANSTREAM 0
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [spanstream_cpp23] fixed-buffer iostream ===\n";

#if LEARN_HAS_SPANSTREAM
#if defined(__cpp_lib_spanstream)
    std::cout << "  __cpp_lib_spanstream=" << __cpp_lib_spanstream << '\n';
#endif

    // ① ospanstream: 写入用户提供的固定 buffer(无堆分配)
    {
        char buf[64]{};
        std::ospanstream oss{std::span<char>{buf}};
        oss << "id=" << 42 << " ok";
        assert(oss.good());
        const auto sp = oss.span();  // 已写区域
        const std::string written(sp.begin(), sp.end());
        assert(written == "id=42 ok");
        std::cout << "  ospanstream: \"" << written << "\"\n";
    }

    // ② ispanstream: 从 span 读
    {
        const char data[] = "3.5 hello";
        std::ispanstream iss{std::span<const char>{data, sizeof(data) - 1}};
        double x = 0;
        std::string word;
        iss >> x >> word;
        assert(x == 3.5);
        assert(word == "hello");
        std::cout << "  ispanstream: " << x << " " << word << '\n';
    }

    // ③ spanstream 双向
    {
        char buf[32]{};
        std::spanstream ss{std::span<char>{buf}};
        ss << 7 << ' ' << 8;
        ss.seekg(0);
        int a = 0, b = 0;
        ss >> a >> b;
        assert(a == 7 && b == 8);
        std::cout << "  spanstream rw: " << a << " " << b << '\n';
    }

    // ④ 对比 stringstream: spanstream 不拥有堆字符串, 适合嵌入式/热路径
    {
        std::vector<char> storage(128, '\0');
        std::ospanstream oss{std::span<char>{storage}};
        for (int i = 0; i < 5; ++i) {
            oss << i << ',';
        }
        const auto sp = oss.span();
        assert(std::string(sp.begin(), sp.end()) == "0,1,2,3,4,");
        std::cout << "  vector-backed ospanstream OK\n";
    }

#else
    // 回退: 用 span + snprintf 演示「固定缓冲格式化」精神
    {
        char buf[64]{};
        const int n = std::snprintf(buf, sizeof(buf), "id=%d ok", 42);
        assert(n > 0);
        assert(std::string(buf) == "id=42 ok");
        std::cout << "  (no <spanstream>) snprintf fallback: " << buf << '\n';
    }
#endif

    std::cout << "[spanstream_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section11/spanstream_cpp23", run>;

}  // namespace
