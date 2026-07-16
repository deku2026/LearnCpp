// SFINAE removes a function-template candidate when signature substitution fails.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section04_compile_time_introspection_traditional
// Item     : sfinae_substitution_failure
// Topic id : part2/stage06/section04/sfinae_substitution_failure
//
// Missing size() is tested in the immediate return-type context rather than activated as an error.

#include "learn/example_support.hpp"

#include <concepts>
#include <cstddef>
#include <string>
#include <vector>

namespace {

template <class T>
auto measured_size(const T& value) -> decltype(value.size()) {
    return value.size();
}

template <class T>
concept HasMeasuredSize = requires(const T& value) {
    { measured_size(value) } -> std::convertible_to<std::size_t>;
};

static_assert(HasMeasuredSize<std::string>);
static_assert(HasMeasuredSize<std::vector<int>>);
static_assert(!HasMeasuredSize<int>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section04/sfinae_substitution_failure"};
    LEARN_EXPECT_EQ(checks, measured_size(std::string{"SFINAE"}), std::size_t{6});
    LEARN_EXPECT_EQ(checks, measured_size(std::vector<int>{1, 2, 3}), std::size_t{3});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section04/sfinae_substitution_failure", run>;

}  // namespace
