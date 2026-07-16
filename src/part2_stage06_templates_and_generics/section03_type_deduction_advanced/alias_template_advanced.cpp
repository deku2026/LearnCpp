// Alias templates expose the result of a reusable type computation concisely.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : alias_template_advanced
// Topic id : part2/stage06/section03/alias_template_advanced
//
// The underlying class template may be partially specialized even though the alias itself cannot.

#include "learn/example_support.hpp"

#include <cstddef>
#include <type_traits>
#include <vector>

namespace {

template <class T>
struct RemoveAllPointers {
    using type = T;
};

template <class T>
struct RemoveAllPointers<T*> {
    using type = typename RemoveAllPointers<T>::type;
};

template <class T>
using RemoveAllPointersT = typename RemoveAllPointers<T>::type;

template <class T>
using ValueVector = std::vector<std::remove_cvref_t<T>>;

static_assert(std::is_same_v<RemoveAllPointersT<int***>, int>);
static_assert(std::is_same_v<ValueVector<const long&>, std::vector<long>>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section03/alias_template_advanced"};
    ValueVector<const int&> values{1, 2, 3};
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, values.back(), 3);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/alias_template_advanced", run>;

}  // namespace
