// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : out_of_bounds_deep
// Topic id : part6/g/section03/out_of_bounds_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <optional>
#include <span>
#include <stdexcept>
#include <vector>

namespace {

template <class T>
[[nodiscard]] constexpr std::optional<T> checked_element(std::span<const T> values, std::size_t index) noexcept {
    if (index >= values.size()) {
        return std::nullopt;
    }
    return values[index];
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/out_of_bounds_deep"};

    const std::vector values{10, 20, 30};
    LEARN_EXPECT_EQ(checks, checked_element<int>(values, 2), std::optional<int>{30});
    LEARN_EXPECT(checks, !checked_element<int>(values, values.size()).has_value());
    LEARN_EXPECT_THROWS(checks, std::out_of_range, values.at(values.size()));

    const auto begin = values.data();
    const auto one_past = begin + values.size();
    LEARN_EXPECT_EQ(checks, one_past - begin, static_cast<std::ptrdiff_t>(values.size()));

#if 0
    int invalid_read = *one_past;  // Forming one-past is valid; dereferencing it is not.
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/out_of_bounds_deep", run>;

}  // namespace
