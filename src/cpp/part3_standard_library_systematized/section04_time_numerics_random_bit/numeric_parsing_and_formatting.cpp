// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库4 · 数值解析/格式化收口
// Stage    : part3_standard_library_systematized
// Section  : section04_time_numerics_random_bit
// Item     : numeric_parsing_and_formatting
// Topic id : part3/section04/numeric_parsing_and_formatting
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string/stol
//   https://en.cppreference.com/w/cpp/utility/to_chars
//   https://en.cppreference.com/w/cpp/io/manip
//
// 要点: stoi/to_string 方便但有异常/分配/locale; 热路径用 charconv;
//       iostream manipulator 粘性状态。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section04/numeric_parsing_and_formatting ===\n";

    // 方便 API
    assert(std::stoi("42") == 42);
    assert(std::stod("3.5") == 3.5);
    assert(std::to_string(42) == "42");
    [[maybe_unused]] std::size_t idx = 0;
    assert(std::stoi("99xyz", &idx) == 99);
    assert(idx == 2);

    [[maybe_unused]] bool threw = false;
    try {
        (void)std::stoi("not-a-number");
    } catch (const std::exception&) {
        threw = true;
    }
    assert(threw);
    std::cout << "[intro] stoi/to_string convenience + exceptions\n";

    // 高性能 charconv
    std::array<char, 32> buf{};
    [[maybe_unused]] auto [p, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), 12345);
    assert(ec == std::errc{});
    assert((std::string_view{buf.data(), static_cast<std::size_t>(p - buf.data())} == "12345"));

    int v = 0;
    const char* s = "6789";
    auto [q, e] = std::from_chars(s, s + 4, v);
    assert(e == std::errc{} && v == 6789);
    (void)q;
    std::cout << "[advanced] charconv path ok\n";

    // iostream 格式化: manipulator 多数粘性
    std::ostringstream oss;
    oss << std::hex << std::setfill('0') << std::setw(4) << 255;
    assert(oss.str() == "00ff" || oss.str() == "00FF");
    oss.str("");
    oss.clear();
    oss << std::dec << 255;  // 需要切回 dec
    assert(oss.str() == "255");
    std::cout << "[expert] iostream manipulators are sticky; prefer format for "
                 "new code\n";

    std::cout << "numeric_parsing_and_formatting: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section04/numeric_parsing_and_formatting", run>;

}  // namespace
