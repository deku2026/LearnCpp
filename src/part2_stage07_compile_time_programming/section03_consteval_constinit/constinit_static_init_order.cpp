// Runnable teaching example
// Doc      : 第2部分-阶段7-编译期编程.md
// Stage    : part2_stage07_compile_time_programming
// Section  : section03_consteval_constinit
// Item     : constinit_static_init_order
// Topic id : part2/stage07/section03/constinit_static_init_order
// References: N4950 [dcl.constinit], [basic.start.static], [basic.start.dynamic]; P1143R2.

#include "learn/example_support.hpp"

namespace {

#if defined(__cpp_constinit) && __cpp_constinit >= 201907L

constexpr int initial_service_port() noexcept {
    return 8'080;
}

// Constant initialization is guaranteed to finish before any dynamic
// initialization. Unlike constexpr, constinit does not make the object const.
constinit int service_port = initial_service_port();
constinit thread_local int request_count = 0;

// Reading a mutable object is not a constant expression, so this represents a
// dependent dynamic initializer. service_port is nevertheless already ready.
int dependent_snapshot = service_port;

#if 0
template <int>
struct PortTag {};
using Rejected = PortTag<service_port>;  // constinit is not a constant-expression guarantee.
constinit int rejected = runtime_only_factory();  // Initializer must be constant.
#endif

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

#if defined(__cpp_constinit) && __cpp_constinit >= 201907L
    ::learn::ExampleChecks checks{"part2/stage07/section03/constinit_static_init_order"};

    LEARN_EXPECT_EQ(checks, dependent_snapshot, 8'080);

    const int original_port = service_port;
    service_port = 9'090;
    LEARN_EXPECT_EQ(checks, service_port, 9'090);
    service_port = original_port;

    const int before = request_count;
    ++request_count;
    LEARN_EXPECT_EQ(checks, request_count, before + 1);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable("part2/stage07/section03/constinit_static_init_order",
                                               "__cpp_constinit >= 201907L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage07/section03/constinit_static_init_order", run>;

}  // namespace
