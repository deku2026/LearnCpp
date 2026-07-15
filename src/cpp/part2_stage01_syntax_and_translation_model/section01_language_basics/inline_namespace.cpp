// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : inline_namespace
// Topic id : part2/stage01/section01/inline_namespace
//
// Covers: inline namespace versioning (v1/v2), parent-scope injection, ABI-friendly selection

#include "learn/topic_registry.hpp"

#include <string_view>
#include <type_traits>

namespace api {

inline namespace v2 {
inline constexpr int version = 2;
struct Config {
    int flags = 2;
};
inline constexpr std::string_view name() {
    return "v2";
}
}  // namespace v2

namespace v1 {
inline constexpr int version = 1;
struct Config {
    int flags = 1;
};
inline constexpr std::string_view name() {
    return "v1";
}
}  // namespace v1

}  // namespace api

namespace {

void demo_basics() {
    // Members of inline namespace are found via the enclosing namespace.
    LEARN_CHECK(api::version == 2);
    LEARN_CHECK(api::v2::version == 2);
    LEARN_CHECK(api::v1::version == 1);
    LEARN_CHECK(api::name() == "v2");
    LEARN_CHECK(api::v1::name() == "v1");
}

void demo_intermediate() {
    api::Config c{};
    LEARN_CHECK(c.flags == 2);

    api::v1::Config old{};
    LEARN_CHECK(old.flags == 1);

    // Explicit nested name always works regardless of which is inline.
    LEARN_CHECK(api::v2::Config{}.flags == 2);
    static_assert(api::v2::version == 2);
    static_assert(api::v1::version == 1);
}

void demo_expert() {
    // Type identity: api::Config is the same as api::v2::Config while v2 is inline.
    static_assert(std::is_same_v<api::Config, api::v2::Config>);
    static_assert(!std::is_same_v<api::Config, api::v1::Config>);

    // Versioning pattern: clients use api::X; library can switch inline vN
    // without changing call sites (ABI still needs care for layout changes).
    const auto take = [](const api::Config& cfg) { return cfg.flags; };
    LEARN_CHECK(take(api::Config{}) == 2);
    LEARN_CHECK(take(api::v2::Config{}) == 2);

    // Nested access remains available for pin-to-version call sites.
    LEARN_CHECK(api::v1::name() == "v1");
    LEARN_CHECK(api::v2::name() == "v2");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/inline_namespace", run>;

}  // namespace
