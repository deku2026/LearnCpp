// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : namespaces
// Topic id : part2/stage01/section01/namespaces
// Reference: N4950 [namespace.def], [namespace.udir], and [basic.lookup.argdep].

#include "learn/example_support.hpp"

#include <cstdlib>

namespace {

namespace vendor_a {
constexpr int status() noexcept {
    return 10;
}
}  // namespace vendor_a

namespace vendor_b {
constexpr int status() noexcept {
    return 20;
}
}  // namespace vendor_b

namespace app::net {
struct Endpoint {
    int x;
    int y;
};

constexpr int manhattan(Endpoint endpoint) noexcept {
    return std::abs(endpoint.x) + std::abs(endpoint.y);
}
}  // namespace app::net

namespace app::net {
inline constexpr int default_port = 443;
}  // namespace app::net

namespace network = app::net;

constexpr int selected_vendor_status() noexcept {
    using vendor_a::status;
    return status();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section01/namespaces"};

    LEARN_EXPECT_EQ(checks, vendor_a::status(), 10);
    LEARN_EXPECT_EQ(checks, vendor_b::status(), 20);
    LEARN_EXPECT_EQ(checks, selected_vendor_status(), 10);
    LEARN_EXPECT_EQ(checks, network::default_port, 443);

    const network::Endpoint endpoint{-3, 4};
    // Unqualified lookup finds app::net::manhattan through the argument's namespace (ADL).
    LEARN_EXPECT_EQ(checks, manhattan(endpoint), 7);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/namespaces", run>;

}  // namespace
