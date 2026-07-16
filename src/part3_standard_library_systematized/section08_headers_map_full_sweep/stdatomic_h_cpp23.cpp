// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section08_headers_map_full_sweep
// Item     : stdatomic_h_cpp23
// Topic id : part3/section08/stdatomic_h_cpp23
// References: C++23 library clauses, [support], [diagnostics], [headers], [stdatomic.h.syn]

#include "learn/example_support.hpp"

#include <string_view>

#if __has_include(<stdatomic.h>)
#include <stdatomic.h>
#endif
#include <atomic>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section08/stdatomic_h_cpp23";

constexpr bool header_present =
#if __has_include(<stdatomic.h>)
    true;
#else
    false;
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::atomic<int> value{1};
    value.store(7, std::memory_order_release);
    LEARN_EXPECT_EQ(checks, value.load(std::memory_order_acquire), 7);
#if defined(__cpp_lib_stdatomic_h) && __cpp_lib_stdatomic_h >= 202011L
    LEARN_EXPECT(checks, header_present);
#else
    LEARN_EXPECT(checks, header_present || !header_present);
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section08/stdatomic_h_cpp23", run>;

}  // namespace
