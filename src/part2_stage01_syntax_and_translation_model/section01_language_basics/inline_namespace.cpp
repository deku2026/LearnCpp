// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : inline_namespace
// Topic id : part2/stage01/section01/inline_namespace
// Reference: N4950 [namespace.def].

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

namespace service {

namespace v1 {
struct Result {
    int status;
};

constexpr int protocol(Result) noexcept {
    return 1;
}
}  // namespace v1

inline namespace v2 {
struct Result {
    int status;
    bool cached;
};

constexpr int protocol(Result) noexcept {
    return 2;
}

constexpr bool succeeded(Result result) noexcept {
    return result.status == 0;
}
}  // namespace v2

}  // namespace service

static_assert(std::is_same_v<service::Result, service::v2::Result>);
static_assert(!std::is_same_v<service::Result, service::v1::Result>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section01/inline_namespace"};

    const service::Result current{0, true};
    const service::v1::Result legacy{0};

    LEARN_EXPECT(checks, service::succeeded(current));
    LEARN_EXPECT_EQ(checks, service::protocol(current), 2);
    LEARN_EXPECT_EQ(checks, service::v1::protocol(legacy), 1);

    // An inline namespace participates in ADL, so this finds service::v2::protocol.
    LEARN_EXPECT_EQ(checks, protocol(current), 2);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/inline_namespace", run>;

}  // namespace
