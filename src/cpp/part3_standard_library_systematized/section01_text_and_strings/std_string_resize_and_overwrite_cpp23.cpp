// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.1 resize_and_overwrite (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_resize_and_overwrite_cpp23
// Topic id : part3/section01/std_string_resize_and_overwrite_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string/resize_and_overwrite
//   P1072R10
//   Feature-test: __cpp_lib_string_resize_and_overwrite >= 202110L
//
// 要点: 预分配后让回调直接写缓冲区, 避免 resize 默认初始化再覆盖的开销。
//       回调签名: size_t op(CharT* p, size_t count); 返回实际写入长度。

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_string_resize_and_overwrite_cpp23 ===\n";

#if defined(__cpp_lib_string_resize_and_overwrite) && __cpp_lib_string_resize_and_overwrite >= 202110L
    std::string buf;
    buf.resize_and_overwrite(5, [](char* p, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            p[i] = static_cast<char>('A' + static_cast<int>(i));
        }
        return n;  // 实际有效长度
    });
    assert(buf == "ABCDE");
    assert(buf.size() == 5);
    std::cout << "[intro] resize_and_overwrite -> " << buf << '\n';

    // 可写少于 count 的长度: 返回值决定最终 size
    std::string partial;
    partial.resize_and_overwrite(16, [](char* p, std::size_t /*n*/) {
        const char* msg = "hi";
        std::memcpy(p, msg, 2);
        return std::size_t{2};
    });
    assert(partial == "hi");
    assert(partial.size() == 2);

    // 典型场景: 对接只写指针+容量 的 C API
    std::string from_c;
    from_c.resize_and_overwrite(32, [](char* p, std::size_t n) {
        // 模拟 snprintf 风格填充
        const char* src = "payload-42";
        const std::size_t len = std::char_traits<char>::length(src);
        const std::size_t write = len < n ? len : n;
        std::memcpy(p, src, write);
        return write;
    });
    assert(from_c == "payload-42");

    std::cout << "[advanced] partial length + C-API style fill ok\n";
    std::cout << "[expert] __cpp_lib_string_resize_and_overwrite=" << __cpp_lib_string_resize_and_overwrite << '\n';
#else
    // 降级: resize 再写(会值初始化)
    std::string buf(5, '\0');
    for (std::size_t i = 0; i < buf.size(); ++i) {
        buf[i] = static_cast<char>('A' + static_cast<int>(i));
    }
    assert(buf == "ABCDE");
    std::cout << "[intro] resize_and_overwrite unavailable; used resize fallback\n";
#endif

    std::cout << "std_string_resize_and_overwrite_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_resize_and_overwrite_cpp23", run>;

}  // namespace
