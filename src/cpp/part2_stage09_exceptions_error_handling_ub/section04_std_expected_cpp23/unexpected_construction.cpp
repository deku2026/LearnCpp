// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : unexpected_construction
// Topic id : part2/stage09/section04/unexpected_construction
//
// Covers: std::unexpected construction for error channel

#include "learn/topic_registry.hpp"

#include <string>
#include <version>

#if defined(__has_include)
#if __has_include(<expected>)
#include <expected>
#endif
#endif
namespace {

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L && __has_include(<expected>)

void demo_basics() {
    std::unexpected u{std::string{"e1"}};
    std::expected<int, std::string> ex{u};
    LEARN_CHECK(!ex.has_value());
    LEARN_CHECK(ex.error() == "e1");
}

void demo_intermediate() {
    auto make_err = [](const char* m) { return std::expected<int, std::string>{std::unexpected<std::string>(m)}; };
    auto e = make_err("boom");
    LEARN_CHECK(e.error() == "boom");
}

void demo_expert() {
    std::expected<std::string, int> e{std::unexpected<int>(404)};
    LEARN_CHECK(!e);
    LEARN_CHECK(e.error() == 404);
}

#else

void demo_basics() {
    // Fallback: error tag + payload.
    bool ok = false;
    std::string err = "e1";
    LEARN_CHECK(!ok);
    LEARN_CHECK(err == "e1");
}

void demo_intermediate() {
    int code = 7;
    LEARN_CHECK(code == 7);
}

void demo_expert() {
    std::string msg = "boom";
    LEARN_CHECK(msg.size() == 4);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section04/unexpected_construction", run>;

}  // namespace
