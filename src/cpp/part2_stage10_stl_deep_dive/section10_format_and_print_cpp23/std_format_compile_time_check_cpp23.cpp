// Topic    : std::format 编译期格式串检查(P2216 DR)
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 15.2②
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : std_format_compile_time_check_cpp23
// Topic id : part2/stage10/section10/std_format_compile_time_check_cpp23
// Refs     : https://en.cppreference.com/w/cpp/utility/format/format
//            https://en.cppreference.com/w/cpp/utility/format/vformat
//            P2216R3

#include "learn/topic_registry.hpp"

#include <cassert>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_format_compile_time_check_cpp23] P2216 ===\n";

    // ① 字面格式串: 编译期检查类型与说明符匹配
    //    错误示例(勿取消注释): std::format("{:d}", "hello"); → 编译失败
    {
        const std::string s = std::format("{:d}", 42);
        assert(s == "42");
        const std::string hex = std::format("{:#x}", 255);
        assert(hex == "0xff" || hex == "0xFF");  // 大小写实现可不同? 标准 lower
        // 标准 format 默认 hex 小写
        assert(std::format("{:#x}", 255) == "0xff");
        std::cout << "  compile-time checked: " << s << " / " << hex << '\n';
    }

    // ② 宽度/精度/填充仍编译期解析
    {
        assert(std::format("{:*>5}", 7) == "****7");
        assert(std::format("{:.3}", 3.14159) == "3.14" || std::format("{:.3f}", 3.14159).starts_with("3.14"));
        // 用明确 f 说明符断言
        assert(std::format("{:.2f}", 3.14159) == "3.14");
        std::cout << "  width/precision OK\n";
    }

    // ③ 运行期格式串: 用 vformat(+ format_args), 无法做完整编译期检查
    // make_format_args 要求左值实参
    {
        const std::string_view runtime_fmt = "{:04d}";
        const int answer = 42;
        const std::string out = std::vformat(runtime_fmt, std::make_format_args(answer));
        assert(out == "0042");
        std::cout << "  vformat(runtime): " << out << '\n';
    }

    // ④ 自动编号 / 手动编号
    {
        assert(std::format("{}-{}", "a", 1) == "a-1");
        assert(std::format("{1} then {0}", "first", "second") == "second then first");
        std::cout << "  indexing OK\n";
    }

    // 教学: 字面 fmt → 编译期安全; 动态 fmt → vformat, 错误推迟到运行期
    std::cout << "[std_format_compile_time_check_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10/std_format_compile_time_check_cpp23", run>;

}  // namespace
