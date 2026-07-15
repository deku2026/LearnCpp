// Topic    : std::source_location（C++20 取代 __FILE__/__LINE__）
// Doc      : 第3部分-标准库系统化.md · 库 7.3
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : source_location_cpp20
// Topic id : part3/section07/source_location_cpp20
// Refs     : https://en.cppreference.com/w/cpp/utility/source_location
//            P1208

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <source_location>
#include <string>
#include <string_view>

namespace {

// ⭐ 默认参数在调用处求值 → 自动捕获调用者位置
void log(std::string_view msg, const std::source_location loc = std::source_location::current()) {
    std::cout << loc.file_name() << ':' << loc.line() << " [" << loc.function_name() << "] " << msg << '\n';
}

std::source_location capture(std::source_location loc = std::source_location::current()) {
    return loc;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [source_location_cpp20] current() ===\n";
    {
        const auto loc = std::source_location::current();
        assert(loc.line() > 0);
        assert(std::string_view{loc.file_name()}.find("source_location") != std::string_view::npos ||
               std::string_view{loc.file_name()}.size() > 0);
        std::cout << "here: " << loc.file_name() << ':' << loc.line() << " col=" << loc.column()
                  << " func=" << loc.function_name() << '\n';
    }

    std::cout << "=== 默认参数捕获调用处 ===\n";
    log("hello from run");  // 行号应落在本行附近

    std::cout << "=== 作为返回值传递 ===\n";
    {
        const auto loc = capture();
        std::cout << "captured line=" << loc.line() << " func=" << loc.function_name() << '\n';
        assert(loc.line() > 0);
    }

    std::cout << "=== 对比宏 ===\n";
    std::cout << "macro form: " << __FILE__ << ':' << __LINE__ << '\n';
    std::cout << "source_location is typed, passable, no macro hygiene issues\n";

    std::cout << "[source_location_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section07/source_location_cpp20", run>;

}  // namespace
