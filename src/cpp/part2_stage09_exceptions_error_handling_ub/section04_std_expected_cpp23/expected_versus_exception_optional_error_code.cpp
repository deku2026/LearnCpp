// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section04_std_expected_cpp23
// Item     : expected_versus_exception_optional_error_code
// Topic id : part2/stage09/section04/expected_versus_exception_optional_error_code
//
// Covers: compare expected vs exception vs optional vs error_code styles

#include "learn/topic_registry.hpp"

#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>
#include <version>

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
#include <expected>
#endif

namespace {

int via_exception(int x) {
    if (x < 0) {
        throw std::invalid_argument("neg");
    }
    return x;
}

std::optional<int> via_optional(int x) {
    if (x < 0) {
        return std::nullopt;
    }
    return x;
}

std::error_code via_error_code(int x, int& out) {
    if (x < 0) {
        out = 0;
        return std::make_error_code(std::errc::invalid_argument);
    }
    out = x;
    return {};
}

void demo_basics() {
    assert(via_exception(3) == 3);
    try {
        via_exception(-1);
        assert(false);
    } catch (const std::invalid_argument&) {
    }
}

void demo_intermediate() {
    assert(via_optional(3).value() == 3);
    assert(!via_optional(-1).has_value());

    int out = 0;
    auto ec = via_error_code(5, out);
    assert(!ec);
    assert(out == 5);
    ec = via_error_code(-1, out);
    assert(ec == std::errc::invalid_argument);
}

void demo_expert() {
#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
    auto via_expected = [](int x) -> std::expected<int, std::string> {
        if (x < 0) {
            return std::unexpected<std::string>("neg");
        }
        return x;
    };
    assert(*via_expected(2) == 2);
    assert(via_expected(-1).error() == "neg");
#else
    // expected unavailable: optional stands in for success/failure without reason detail.
    assert(via_optional(2).value() == 2);
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

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part2/stage09/section04/expected_versus_exception_optional_error_code", run>;

}  // namespace
