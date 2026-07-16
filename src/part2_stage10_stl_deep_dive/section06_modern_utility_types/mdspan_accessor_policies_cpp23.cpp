// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_accessor_policies_cpp23
// Topic id : part2/stage10/section06/mdspan_accessor_policies_cpp23

#ifdef __has_include
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARN_HAS_MDSPAN_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/mdspan_accessor_policies_cpp23";

struct CountingAccessorModel {
    int* accesses;

    [[nodiscard]] int& access(int* pointer, std::size_t offset) const noexcept {
        ++*accesses;
        return pointer[offset];
    }
};

#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L

struct CountingAccessor {
    using offset_policy = CountingAccessor;
    using element_type = int;
    using reference = int&;
    using data_handle_type = int*;

    std::size_t* accesses{};

    [[nodiscard]] constexpr reference access(data_handle_type pointer, std::size_t offset) const noexcept {
        ++*accesses;
        return pointer[offset];
    }

    [[nodiscard]] constexpr data_handle_type offset(data_handle_type pointer, std::size_t amount) const noexcept {
        return pointer + amount;
    }
};

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    std::array values{0, 1, 2, 3, 4, 5};
    int model_accesses{};
    const CountingAccessorModel model{&model_accesses};
    model.access(values.data(), 4) = 40;
    LEARN_EXPECT_EQ(checks, values[4], 40);
    LEARN_EXPECT_EQ(checks, model_accesses, 1);

#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    using Extents = std::extents<std::size_t, 2, 3>;
    using Mapping = std::layout_right::mapping<Extents>;
    using View = std::mdspan<int, Extents, std::layout_right, CountingAccessor>;

    std::size_t accesses{};
    const View view{values.data(), Mapping{Extents{}}, CountingAccessor{&accesses}};
    LEARN_EXPECT_EQ(checks, (view[1, 2]), 5);
    LEARN_EXPECT_EQ(checks, accesses, 1U);
    view[0, 1] = 20;
    LEARN_EXPECT_EQ(checks, accesses, 2U);
    LEARN_EXPECT_EQ(checks, values[1], 20);
    LEARN_EXPECT_EQ(checks, view.data_handle(), values.data());
    LEARN_EXPECT_EQ(checks, view.mapping()(1, 2), 5U);
    LEARN_EXPECT_EQ(checks, view.accessor().accesses, &accesses);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_mdspan >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_accessor_policies_cpp23", run>;

}  // namespace
