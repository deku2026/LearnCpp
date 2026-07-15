// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_variant_as_error
// Topic id : part2/stage09/section05/std_variant_as_error
//
// Covers: variant<Value, Error> as pre-expected result type

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

namespace {

struct Error {
    std::string msg;
};

using Result = std::variant<int, Error>;

Result divide(int a, int b) {
    if (b == 0) {
        return Error{"div0"};
    }
    return a / b;
}

void demo_basics() {
    auto r = divide(10, 2);
    assert(std::holds_alternative<int>(r));
    assert(std::get<int>(r) == 5);
}

void demo_intermediate() {
    auto r = divide(1, 0);
    assert(std::holds_alternative<Error>(r));
    assert(std::get<Error>(r).msg == "div0");
}

void demo_expert() {
    auto r = divide(9, 3);
    int value = std::visit(
        [](auto&& x) -> int {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, int>) {
                return x;
            } else {
                return -1;
            }
        },
        r);
    assert(value == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_variant_as_error", run>;

}  // namespace
