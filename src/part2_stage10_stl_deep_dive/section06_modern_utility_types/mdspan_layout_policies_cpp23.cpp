// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_layout_policies_cpp23
// Topic id : part2/stage10/section06/mdspan_layout_policies_cpp23

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

constexpr std::string_view topic_id = "part2/stage10/section06/mdspan_layout_policies_cpp23";

[[nodiscard]] constexpr std::size_t row_major_offset(std::size_t row, std::size_t column,
                                                     std::size_t columns) noexcept {
    return row * columns + column;
}

[[nodiscard]] constexpr std::size_t column_major_offset(std::size_t row, std::size_t column,
                                                        std::size_t rows) noexcept {
    return column * rows + row;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    static_assert(row_major_offset(1, 2, 4) == 6);
    static_assert(column_major_offset(1, 2, 3) == 7);
    LEARN_EXPECT_EQ(checks, row_major_offset(2, 3, 4), 11U);
    LEARN_EXPECT_EQ(checks, column_major_offset(2, 3, 3), 11U);

#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    using Extents = std::extents<std::size_t, 3, 4>;
    using RowMapping = std::layout_right::mapping<Extents>;
    using ColumnMapping = std::layout_left::mapping<Extents>;

    constexpr RowMapping rows{Extents{}};
    constexpr ColumnMapping columns{Extents{}};
    static_assert(rows(1, 2) == 6);
    static_assert(columns(1, 2) == 7);
    static_assert(rows.required_span_size() == 12);
    static_assert(columns.required_span_size() == 12);
    static_assert(RowMapping::is_always_unique() && RowMapping::is_always_exhaustive());
    static_assert(ColumnMapping::is_always_unique() && ColumnMapping::is_always_exhaustive());

    std::array<int, 12> storage{};
    std::iota(storage.begin(), storage.end(), 0);
    const std::mdspan<int, Extents, std::layout_right> row_view{storage.data()};
    const std::mdspan<int, Extents, std::layout_left> column_view{storage.data()};
    LEARN_EXPECT_EQ(checks, (row_view[1, 2]), 6);
    LEARN_EXPECT_EQ(checks, (column_view[1, 2]), 7);
    LEARN_EXPECT_EQ(checks, row_view.mapping().stride(0), 4U);
    LEARN_EXPECT_EQ(checks, row_view.mapping().stride(1), 1U);
    LEARN_EXPECT_EQ(checks, column_view.mapping().stride(0), 1U);
    LEARN_EXPECT_EQ(checks, column_view.mapping().stride(1), 3U);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_mdspan >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_layout_policies_cpp23", run>;

}  // namespace
