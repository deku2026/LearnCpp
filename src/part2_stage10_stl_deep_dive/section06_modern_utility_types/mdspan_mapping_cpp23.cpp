// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_mapping_cpp23
// Topic id : part2/stage10/section06/mdspan_mapping_cpp23

#ifdef __has_include
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARN_HAS_MDSPAN_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <numeric>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/mdspan_mapping_cpp23";

struct StridedMappingModel {
    std::size_t rows;
    std::size_t columns;
    std::size_t row_stride;
    std::size_t column_stride;

    [[nodiscard]] constexpr std::size_t operator()(std::size_t row, std::size_t column) const noexcept {
        return row * row_stride + column * column_stride;
    }

    [[nodiscard]] constexpr std::size_t required_span_size() const noexcept {
        if (rows == 0 || columns == 0) {
            return 0;
        }
        return (*this)(rows - 1U, columns - 1U) + 1U;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    constexpr StridedMappingModel model{2, 3, 4, 1};
    static_assert(model(1, 2) == 6);
    static_assert(model.required_span_size() == 7);
    LEARN_EXPECT_EQ(checks, model(1, 0), 4U);

#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    using Extents = std::dextents<std::size_t, 2>;
    using Mapping = std::layout_stride::mapping<Extents>;

    constexpr Extents extents{2, 3};
    constexpr std::array<std::size_t, 2> strides{4, 1};
    constexpr Mapping mapping{extents, strides};
    static_assert(mapping(1, 2) == 6);
    static_assert(mapping.required_span_size() == 7);
    static_assert(mapping.is_unique());
    static_assert(mapping.is_strided());
    static_assert(!mapping.is_exhaustive());  // Offset 3 is padding.

    std::array<int, 7> storage{};
    std::iota(storage.begin(), storage.end(), 10);
    const std::mdspan<int, Extents, std::layout_stride> view{storage.data(), mapping};
    LEARN_EXPECT_EQ(checks, (view[0, 0]), 10);
    LEARN_EXPECT_EQ(checks, (view[1, 0]), 14);
    LEARN_EXPECT_EQ(checks, (view[1, 2]), 16);
    LEARN_EXPECT_EQ(checks, view.mapping().stride(0), 4U);
    LEARN_EXPECT_EQ(checks, view.mapping().stride(1), 1U);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_mdspan >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_mapping_cpp23", run>;

}  // namespace
