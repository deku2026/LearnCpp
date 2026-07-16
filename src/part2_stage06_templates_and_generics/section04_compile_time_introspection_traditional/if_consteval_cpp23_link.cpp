// C++23 if consteval selects by evaluation context, not by a type trait.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : if_consteval_cpp23_link
// Topic id : part2/stage06/section04/if_consteval_cpp23_link
//
// P1938R3 is protected by its SD-6 macro before the syntax is parsed.

#include "learn/example_support.hpp"

namespace {

#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L

constexpr int context_sensitive(int value) {
    if consteval {
        return value * value;
    } else {
        return value + 1'000;
    }
}

static_assert(context_sensitive(3) == 9);

#endif

int run(int argc, char** argv) {
    (void)argv;
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
    learn::ExampleChecks checks{"part2/stage06/section04/if_consteval_cpp23_link"};
    const int runtime_input = argc >= 0 ? 3 : 4;
    LEARN_EXPECT_EQ(checks, context_sensitive(runtime_input), 1'003);
    return checks.result();
#else
    (void)argc;
    return learn::ExampleChecks::unavailable("part2/stage06/section04/if_consteval_cpp23_link",
                                             "if consteval (__cpp_if_consteval >= 202106L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/if_consteval_cpp23_link", run>;

}  // namespace
