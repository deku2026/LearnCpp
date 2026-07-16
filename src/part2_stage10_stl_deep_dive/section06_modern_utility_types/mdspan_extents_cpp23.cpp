// LearnCpp example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section06_modern_utility_types
// Item     : mdspan_extents_cpp23
// Topic id : part2/stage10/section06/mdspan_extents_cpp23

#ifdef __has_include
#if __has_include(<mdspan>)
#include <mdspan>
#define LEARN_HAS_MDSPAN_HEADER 1
#endif
#endif

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>

namespace {

constexpr std::string_view topic_id = "part2/stage10/section06/mdspan_extents_cpp23";

template <std::size_t StaticRows>
class ExtentsModel {
public:
    explicit constexpr ExtentsModel(std::size_t columns) noexcept : columns_(columns) {}

    [[nodiscard]] static consteval std::size_t rank() noexcept { return 2; }
    [[nodiscard]] static consteval std::size_t rank_dynamic() noexcept { return 1; }
    [[nodiscard]] constexpr std::size_t extent(std::size_t dimension) const noexcept {
        return dimension == 0 ? StaticRows : columns_;
    }

private:
    std::size_t columns_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    constexpr ExtentsModel<3> model{5};
    static_assert(model.rank() == 2 && model.rank_dynamic() == 1);
    LEARN_EXPECT_EQ(checks, model.extent(0), 3U);
    LEARN_EXPECT_EQ(checks, model.extent(1), 5U);

#if defined(LEARN_HAS_MDSPAN_HEADER) && defined(__cpp_lib_mdspan) && __cpp_lib_mdspan >= 202207L
    using Fixed = std::extents<std::size_t, 3, 4>;
    using Mixed = std::extents<std::size_t, 3, std::dynamic_extent>;
    using Dynamic = std::dextents<std::size_t, 2>;

    constexpr Fixed fixed;
    constexpr Mixed mixed{5};
    constexpr Dynamic dynamic{2, 6};

    static_assert(Fixed::rank() == 2 && Fixed::rank_dynamic() == 0);
    static_assert(Mixed::rank() == 2 && Mixed::rank_dynamic() == 1);
    static_assert(Fixed::static_extent(0) == 3);
    static_assert(Fixed::static_extent(1) == 4);
    static_assert(Mixed::static_extent(1) == std::dynamic_extent);
    static_assert(std::is_same_v<Fixed::index_type, std::size_t>);

    LEARN_EXPECT_EQ(checks, fixed.extent(0), 3U);
    LEARN_EXPECT_EQ(checks, fixed.extent(1), 4U);
    LEARN_EXPECT_EQ(checks, mixed.extent(0), 3U);
    LEARN_EXPECT_EQ(checks, mixed.extent(1), 5U);
    LEARN_EXPECT_EQ(checks, dynamic.extent(0), 2U);
    LEARN_EXPECT_EQ(checks, dynamic.extent(1), 6U);
    return checks.result();
#else
    if (checks.result() != 0) {
        return checks.result();
    }
    return learn::ExampleChecks::unavailable(topic_id, "C++23 __cpp_lib_mdspan >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/mdspan_extents_cpp23", run>;

}  // namespace
