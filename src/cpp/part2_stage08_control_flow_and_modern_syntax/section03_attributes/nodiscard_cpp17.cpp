// LearnCpp topic example
// Doc      : part2-stage08-control-flow-and-modern-syntax.md
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : nodiscard_cpp17
// Topic id : part2/stage08/section03/nodiscard_cpp17
//
// Covers: [[nodiscard]] on functions and types

#include "learn/topic_registry.hpp"

#include <string>

namespace {

[[nodiscard]] int compute() {
    return 42;
}

[[nodiscard("check error code")]] int open_resource() {
    return 0;
}

struct [[nodiscard]] Error {
    int code = 0;
};

Error make_error(int c) {
    return Error{c};
}

void demo_basics() {
    int v = compute();
    LEARN_CHECK(v == 42);
    // Discarding compute() would typically warn; we always use the result.
}

void demo_intermediate() {
    int ec = open_resource();
    LEARN_CHECK(ec == 0);
}

void demo_expert() {
    Error e = make_error(7);
    LEARN_CHECK(e.code == 7);

    // C++20: nodiscard can carry a reason string (message for diagnostics).
    auto x = open_resource();
    LEARN_CHECK(x == 0);
    (void)std::string{"used"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/nodiscard_cpp17", run>;

}  // namespace
