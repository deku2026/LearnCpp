// Topic    : headers map · 字符串/文本/本地化
// Doc      : 第3部分-标准库系统化.md · headers map
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : text_headers
// Topic id : part3/section08/text_headers
// Headers  : <string> <string_view> <charconv> <format> <print>
//            <regex> <locale> <cctype>

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cctype>
#include <charconv>
#include <format>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <string_view>
#include <system_error>

#if defined(__has_include)
#if __has_include(<print>)
#include <print>
#define LEARN_HAS_PRINT 1
#endif
#endif
#ifndef LEARN_HAS_PRINT
#define LEARN_HAS_PRINT 0
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [text_headers] map probe ===\n";
    std::cout << "<string>/<string_view>/<charconv>/<format>/<print>\n";
    std::cout << "<regex>/<locale>/<cctype>\n";

    std::string s = "Hello";
    assert(s.contains('e'));
    std::string_view sv = s;
    assert(sv.starts_with("He"));

    char buf[16]{};
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), 42);
    assert(ec == std::errc{});
    assert(std::string_view(buf, ptr) == "42");

    const auto line = std::format("{}-{}", s, 1);
    assert(line == "Hello-1");
    std::cout << "format: " << line << '\n';

#if LEARN_HAS_PRINT
    std::println("print says {}", 123);
#else
    std::cout << std::format("print fallback {}\n", 123);
#endif

    assert(std::regex_match("abc123", std::regex{R"([a-z]+\d+)"}));
    std::cout << "regex match ok\n";

    std::locale loc = std::locale::classic();
    std::cout << "classic locale name=" << loc.name() << '\n';
    assert(std::isdigit(static_cast<unsigned char>('7')));

    std::cout << "[text_headers] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section08/text_headers", run>;

}  // namespace
