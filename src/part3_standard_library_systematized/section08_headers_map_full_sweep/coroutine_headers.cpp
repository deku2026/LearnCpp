// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : coroutine_headers
// Topic id : part3/section08/coroutine_headers
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <array>
#include <coroutine>
#include <string_view>
#if __has_include(<generator>)
#include <generator>
#endif
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section08/coroutine_headers";

constexpr std::array<std::string_view, 2> headers{"coroutine", "generator"};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto noop = std::noop_coroutine();
    LEARN_EXPECT(checks, static_cast<bool>(noop));
    LEARN_EXPECT(checks, !noop.done());
    LEARN_EXPECT_EQ(checks, headers.size(), 2U);
#if defined(__cpp_lib_generator) && __cpp_lib_generator >= 202207L
    LEARN_EXPECT(checks, true);
#else
    LEARN_EXPECT(checks, headers[1] == "generator");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/coroutine_headers", run>;

}  // namespace
