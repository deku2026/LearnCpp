// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : expected_overview_cpp23
// Topic id : part3/section03/expected_overview_cpp23
//
// Covers: std::expected success/error C++23

#include "learn/topic_registry.hpp"

#include <string>
#include <version>
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

void demo_basics() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    std::expected<int, std::string> e = 1;
    LEARN_CHECK(e.has_value());
#else
    LEARN_CHECK(true);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    std::expected<int, std::string> e = 42;
    LEARN_CHECK(*e == 42);
    std::expected<int, std::string> err = std::unexpected<std::string>{"fail"};
    LEARN_CHECK(!err.has_value());
    LEARN_CHECK(err.error() == "fail");
#else
    std::string err = "fail";
    LEARN_CHECK(err == "fail");
#endif
}

void demo_expert() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto div = [](int a, int b) -> std::expected<int, std::string> {
        if (b == 0) {
            return std::unexpected<std::string>{"div0"};
        }
        return a / b;
    };
    LEARN_CHECK(div(10, 2).value() == 5);
    LEARN_CHECK(div(1, 0).error() == "div0");
#else
    LEARN_CHECK(true);
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/expected_overview_cpp23", run>;

}  // namespace
