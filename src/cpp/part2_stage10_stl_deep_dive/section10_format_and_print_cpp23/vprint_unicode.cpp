// Topic    : std::vprint_unicode / vprint_nonunicode — print 的 Unicode 路径
// Doc      : 第2部分-阶段10-STL深潜.md · 步骤 15.1(Unicode 真输出)
// Stage    : part2_stage10_stl_deep_dive
// Section  : section10_format_and_print_cpp23
// Item     : vprint_unicode
// Topic id : part2/stage10/section10/vprint_unicode
// Refs     : https://en.cppreference.com/w/cpp/io/vprint_unicode
//            https://en.cppreference.com/w/cpp/io/vprint_nonunicode
//            P2093 / P2539

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdio>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
#include <print>
#define LEARN_HAS_VPRINT 1
#else
#define LEARN_HAS_VPRINT 0
#endif

namespace {

// 运行期格式 + format_args → 走 vprint_* 底层
void demo_vprint(std::FILE* stream, std::string_view fmt, const auto&... args) {
#if LEARN_HAS_VPRINT
    // vprint_unicode: 在支持的平台上按 Unicode 终端约定写出
    // (Windows 上避免传统代码页 mojibake)
    std::vprint_unicode(stream, fmt, std::make_format_args(args...));
#else
    const std::string s = std::vformat(fmt, std::make_format_args(args...));
    std::fputs(s.c_str(), stream);
#endif
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [vprint_unicode] Unicode-aware printing path ===\n";

    // ① 内容本身是 UTF-8 源码字符串; format 产生 UTF-8 字节序列
    {
        const std::string msg = std::format("你好, {}! π≈{:.2f}\n", "C++23", 3.14159);
        assert(msg.find("你好") != std::string::npos);
        assert(msg.find("3.14") != std::string::npos);
        // 写出: 有 vprint_unicode 时走 Unicode API; 否则 fputs
        demo_vprint(stdout, "  {}", msg);
    }

    // ② 对比: vformat 得到字符串 vs 直接 vprint
    // make_format_args 要求左值
    {
        const std::string_view fmt = "  id={:04d} name={}\n";
        const int id = 7;
        const std::string name = "gizmo";
        const std::string built = std::vformat(fmt, std::make_format_args(id, name));
        assert(built == "  id=0007 name=gizmo\n");

#if LEARN_HAS_VPRINT
        std::cout << "  __cpp_lib_print=" << __cpp_lib_print << '\n';
        std::vprint_unicode(stdout, fmt, std::make_format_args(id, name));
        // nonunicode 路径: 按字节原样写(不做 Unicode 终端特殊处理)
        const std::string tag = "bytes-as-is";
        std::vprint_nonunicode(stdout, "  nonunicode path: {}\n", std::make_format_args(tag));
#else
        std::fputs(built.c_str(), stdout);
        std::cout << "  (no vprint_*) used vformat+fputs fallback\n";
#endif
    }

    // 教学:
    // - print/println 内部会选 unicode / nonunicode 路径
    // - 应用层优先 println; 需要运行期 fmt + 锁定流时用 vprint_unicode
    std::cout << "[vprint_unicode] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section10/vprint_unicode", run>;

}  // namespace
