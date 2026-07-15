// LearnCpp topic example
// Doc      : part2-stage03-functions-overloading-lambdas.md
// Stage    : part2_stage03_functions_overloading_lambdas
// Section  : section05_callables_and_function_wrappers
// Item     : std_bind_back_cpp23
// Topic id : part2/stage03/section05/std_bind_back_cpp23
//
// Covers: std::bind_back (C++23) trailing partial application

#include "learn/topic_registry.hpp"

#include <functional>
#include <string>

namespace {

int sub(int a, int b) {
    return a - b;
}

int add3(int a, int b, int c) {
    return a + b + c;
}

void demo_basics() {
#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    auto sub_from = std::bind_back(sub, 3);
    LEARN_CHECK(sub_from(10) == 7);
#else
    auto sub_from = [](int a) { return sub(a, 3); };
    LEARN_CHECK(sub_from(10) == 7);
#endif
}

void demo_intermediate() {
#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    auto plus_c = std::bind_back(add3, 3);
    LEARN_CHECK(plus_c(1, 2) == 6);
    auto plus_bc = std::bind_back(add3, 2, 3);
    LEARN_CHECK(plus_bc(1) == 6);
#else
    auto plus_c = [](int a, int b) { return add3(a, b, 3); };
    LEARN_CHECK(plus_c(1, 2) == 6);
#endif
}

void demo_expert() {
    auto wrap = [](const std::string& body, const std::string& suffix) { return body + suffix; };
#if defined(__cpp_lib_bind_back) && __cpp_lib_bind_back >= 202202L
    auto with_bang = std::bind_back(wrap, std::string{"!"});
    LEARN_CHECK(with_bang(std::string{"ok"}) == "ok!");
#else
    auto with_bang = [&](const std::string& body) { return wrap(body, "!"); };
    LEARN_CHECK(with_bang(std::string{"ok"}) == "ok!");
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_back_cpp23", run>;

}  // namespace
