// A pack expansion repeats its pattern once for every pack element.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : pack_expansion
// Topic id : part2/stage06/section02/pack_expansion
//
// The examples expand member calls into an array and arithmetic into a tuple.

#include "learn/example_support.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace {

template <class... Containers>
auto sizes_of(const Containers&... containers) {
    return std::array<std::size_t, sizeof...(Containers)>{containers.size()...};
}

template <class... Values>
auto doubled_values(Values... values) {
    return std::tuple{(values + values)...};
}

static_assert(std::is_same_v<decltype(doubled_values(1, 2.0)), std::tuple<int, double>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section02/pack_expansion"};
    const auto sizes = sizes_of(std::string{"abc"}, std::vector<int>{1, 2}, std::array<int, 4>{});
    LEARN_EXPECT_EQ(checks, sizes, (std::array<std::size_t, 3>{3, 2, 4}));

    const auto doubled = doubled_values(3, 1.5, std::string{"x"});
    LEARN_EXPECT_EQ(checks, std::get<0>(doubled), 6);
    LEARN_EXPECT_EQ(checks, std::get<1>(doubled), 3.0);
    LEARN_EXPECT_EQ(checks, std::get<2>(doubled), std::string{"xx"});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/pack_expansion", run>;

}  // namespace
