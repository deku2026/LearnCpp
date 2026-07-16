// C++17 [*this] copies the current object into the closure.
// Doc      : 第2部分-阶段3-函数-重载-lambda.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section03_lambdas_core
// Item     : capture_star_this_cpp17
// Topic id : part2/stage03/section03/capture_star_this_cpp17
//
// The copied-object snapshot can outlive its source.  A [this] view is shown
// only while the source object remains alive.

#include "learn/example_support.hpp"

#include <type_traits>

namespace {

struct Reading {
    int value{};

    auto live_view() {
        return [this] { return value; };
    }

    auto snapshot() const {
        return [*this] { return value; };
    }
};

auto make_owned_snapshot() {
    Reading temporary{19};
    return temporary.snapshot();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage03/section03/capture_star_this_cpp17"};

    Reading reading{7};
    auto view = reading.live_view();
    auto copy = reading.snapshot();
    static_assert(std::is_copy_constructible_v<decltype(copy)>);

    reading.value = 11;
    LEARN_EXPECT_EQ(checks, view(), 11);
    LEARN_EXPECT_EQ(checks, copy(), 7);

    const auto escaped_copy = make_owned_snapshot();
    LEARN_EXPECT_EQ(checks, escaped_copy(), 19);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_star_this_cpp17", run>;

}  // namespace
