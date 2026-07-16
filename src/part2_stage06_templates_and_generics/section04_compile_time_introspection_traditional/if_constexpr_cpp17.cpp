// C++17 if constexpr discards the branch not selected for an instantiation.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : if_constexpr_cpp17
// Topic id : part2/stage06/section04/if_constexpr_cpp17
//
// The string-only size() expression is never instantiated for an integer.

#include "learn/example_support.hpp"

#include <cstddef>
#include <string>
#include <type_traits>

namespace {

template <class T>
constexpr std::size_t magnitude(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        return static_cast<std::size_t>(value < 0 ? -value : value);
    } else {
        return value.size();
    }
}

static_assert(magnitude(-7) == 7);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section04/if_constexpr_cpp17"};
    LEARN_EXPECT_EQ(checks, magnitude(-9), std::size_t{9});
    LEARN_EXPECT_EQ(checks, magnitude(std::string{"compile"}), std::size_t{7});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/if_constexpr_cpp17", run>;

}  // namespace
