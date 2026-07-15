// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section02_variadic_functions
// Item     : legacy_c_variadic
// Topic id : part2/stage03/section02/legacy_c_variadic
//
// Covers: C-style va_list carefully; prefer fold/variadic templates

#include "learn/topic_registry.hpp"

#include <cstdarg>
#include <string>

namespace {

// SAFE demo: fixed count + only int args (no type mismatch UB).
int sum_n(int count, ...) {
    va_list ap;
    va_start(ap, count);
    int total = 0;
    for (int i = 0; i < count; ++i) {
        total += va_arg(ap, int);
    }
    va_end(ap);
    return total;
}

// Prefer type-safe modern path (shown for contrast).
template <class... Args>
int sum_fold(Args... args) {
    return (0 + ... + static_cast<int>(args));
}

void demo_basics() {
    LEARN_CHECK(sum_n(3, 1, 2, 3) == 6);
    LEARN_CHECK(sum_n(0) == 0);
}

void demo_intermediate() {
    LEARN_CHECK(sum_n(4, 10, 20, 30, 40) == 100);
    // float promotes to double in C varargs — never va_arg as float.
    // Here we only pass ints: safe and portable.
    LEARN_CHECK(sum_n(2, -1, 1) == 0);
}

void demo_expert() {
    // Modern replacement: fold expression — type-checked at compile time.
    LEARN_CHECK(sum_fold(1, 2, 3, 4) == 10);
    LEARN_CHECK(sum_fold() == 0);

    // C varargs: wrong va_arg type is UB. Do not use for new APIs.
    const std::string note = "use packs, not va_list";
    LEARN_CHECK(!note.empty());
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/legacy_c_variadic", run>;

}  // namespace
