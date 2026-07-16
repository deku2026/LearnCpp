// A user-defined deduction guide maps constructor arguments to a class specialization.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : deduction_guides
// Topic id : part2/stage06/section03/deduction_guides
//
// Iterator arguments do not directly expose RangeBox<T>, so the guide supplies iter_value_t.

#include "learn/example_support.hpp"

#include <iterator>
#include <type_traits>
#include <vector>

namespace {

template <class T>
struct RangeBox {
    std::vector<T> values;

    template <class Iterator>
    RangeBox(Iterator first, Iterator last) : values(first, last) {}
};

template <class Iterator>
RangeBox(Iterator, Iterator) -> RangeBox<std::iter_value_t<Iterator>>;

static_assert(
    std::is_same_v<decltype(RangeBox{static_cast<int*>(nullptr), static_cast<int*>(nullptr)}), RangeBox<int>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section03/deduction_guides"};
    const std::vector<long> source{4, 5, 6};
    RangeBox copied{source.begin(), source.end()};
    static_assert(std::is_same_v<decltype(copied), RangeBox<long>>);
    LEARN_EXPECT_EQ(checks, copied.values, source);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/deduction_guides", run>;

}  // namespace
