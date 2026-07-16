// Runnable teaching example
// Doc      : 第2部分-阶段11-并发与多线程.md
// Stage    : part2_stage11_concurrency_and_threading
// Section  : section03_atomics_and_memory_model
// Item     : c_atomic_interop_cpp23
// Topic id : part2/stage11/section03/c_atomic_interop_cpp23
// References: C++23 [intro.races], [atomics.order], [thread.stoptoken]

#include "learn/example_support.hpp"

#include <atomic>
#include <string_view>
#include <version>

#if defined(__cpp_lib_stdatomic_h) && __cpp_lib_stdatomic_h >= 202011L && __has_include(<stdatomic.h>)
#include <stdatomic.h>
#endif

namespace {

constexpr std::string_view kTopic = "part2/stage11/section03/c_atomic_interop_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_lib_stdatomic_h) && __cpp_lib_stdatomic_h >= 202011L && __has_include(<stdatomic.h>)
    ::learn::ExampleChecks checks{kTopic};
    _Atomic(int) value{40};
    LEARN_EXPECT_EQ(checks, atomic_fetch_add(&value, 2), 40);
    LEARN_EXPECT_EQ(checks, atomic_load(&value), 42);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "C++23 <stdatomic.h> compatibility");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage11/section03/c_atomic_interop_cpp23", run>;

}  // namespace
