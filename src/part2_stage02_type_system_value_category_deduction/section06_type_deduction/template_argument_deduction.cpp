// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section06_type_deduction
// Item     : template_argument_deduction
// Topic id : part2/stage02/section06/template_argument_deduction
//

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace {

template <class T>
constexpr std::type_identity<T> deduce_by_value(T) {
    return {};
}

template <class T>
constexpr std::type_identity<T> deduce_by_reference(T&) {
    return {};
}

template <class T>
constexpr std::type_identity<T> deduce_forwarding(T&&) {
    return {};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const int constant = 4;
    int array[3]{};

    using ValueT = typename decltype(deduce_by_value(constant))::type;
    using ReferenceT = typename decltype(deduce_by_reference(constant))::type;
    using ArrayByValueT = typename decltype(deduce_by_value(array))::type;
    using ArrayByReferenceT = typename decltype(deduce_by_reference(array))::type;
    using ForwardingT = typename decltype(deduce_forwarding(constant))::type;

    static_assert(std::is_same_v<ValueT, int>);
    static_assert(std::is_same_v<ReferenceT, const int>);
    static_assert(std::is_same_v<ArrayByValueT, int*>);
    static_assert(std::is_same_v<ArrayByReferenceT, int[3]>);
    static_assert(std::is_same_v<ForwardingT, const int&>);

    learn::ExampleChecks checks{"part2/stage02/section06/template_argument_deduction"};
    LEARN_EXPECT_EQ(checks, std::extent_v<ArrayByReferenceT>, std::size_t{3});
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section06/template_argument_deduction", run>;

}  // namespace
