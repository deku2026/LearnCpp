// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section07_diagnostics_and_debugging
// Item     : source_location_cpp20
// Topic id : part3/section07/source_location_cpp20
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <cstdint>
#include <source_location>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section07/source_location_cpp20";

struct CallSite {
    std::string_view file;
    std::string_view function;
    std::uint_least32_t line;
};

CallSite capture(std::source_location location = std::source_location::current()) {
    return {location.file_name(), location.function_name(), location.line()};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto site = capture();
    LEARN_EXPECT(checks, site.file.contains("source_location_cpp20.cpp"));
    LEARN_EXPECT(checks, site.function.contains("run"));
    LEARN_EXPECT(checks, site.line > 0);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section07/source_location_cpp20", run>;

}  // namespace
