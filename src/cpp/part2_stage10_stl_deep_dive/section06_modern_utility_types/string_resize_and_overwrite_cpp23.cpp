// Topic     : string::resize_and_overwrite (C++23, P1072)
// Doc       : 第2部分-阶段10 · 步骤 7.1
// cppreference: https://en.cppreference.com/cpp/string/basic_string/resize_and_overwrite
//
// 要点: 直接填充未初始化缓冲, 跳过先清零再覆盖; 返回实际写入长度。

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_resize_and_overwrite_cpp23] ===\n";

    std::string s;
    s.resize_and_overwrite(10, [](char* buf, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) buf[i] = static_cast<char>('0' + (i % 10));
        return n;  // 实际使用长度
    });
    assert(s == "0123456789");
    assert(s.size() == 10);

    // 可写入少于 n
    std::string t = "prefix";
    t.resize_and_overwrite(20, [](char* buf, std::size_t n) {
        // 注意: 调用时原内容不保证保留; 从零构造缓冲
        (void)n;
        const char* msg = "hello";
        const std::size_t len = std::strlen(msg);
        std::memcpy(buf, msg, len);
        return len;
    });
    assert(t == "hello");
    assert(t.size() == 5);

    // 模拟 C API 填充 (snprintf)
    std::string formatted;
    const int value = 42;
    formatted.resize_and_overwrite(32, [](char* buf, std::size_t n) {
        const int written = std::snprintf(buf, n, "value=%d", value);
        assert(written >= 0);
        return static_cast<std::size_t>(written);
    });
    assert(formatted == "value=42");

    std::cout << "[resize_and_overwrite] s=" << s << " formatted=" << formatted << '\n';
    std::cout << "string_resize_and_overwrite_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_resize_and_overwrite_cpp23", run>;

}  // namespace
