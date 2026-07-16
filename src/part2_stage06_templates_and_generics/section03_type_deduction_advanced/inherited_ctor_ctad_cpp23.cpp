// P2582R1 lets C++23 CTAD use constructors inherited from a dependent base.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section03_type_deduction_advanced
// Item     : inherited_ctor_ctad_cpp23
// Topic id : part2/stage06/section03/inherited_ctor_ctad_cpp23
//
// The adopted SD-6 value is required before the parser sees the deduction example.

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 202207L

template <class T>
struct BaseValue {
    T value;

    explicit BaseValue(T initial) : value(std::move(initial)) {}
};

template <class T>
struct DerivedValue : BaseValue<T> {
    using BaseValue<T>::BaseValue;
};

static_assert(std::is_same_v<decltype(DerivedValue{42}), DerivedValue<int>>);

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 202207L
    learn::ExampleChecks checks{"part2/stage06/section03/inherited_ctor_ctad_cpp23"};
    DerivedValue value{42};
    LEARN_EXPECT_EQ(checks, value.value, 42);
    return checks.result();
#else
    return learn::ExampleChecks::unavailable("part2/stage06/section03/inherited_ctor_ctad_cpp23",
                                             "CTAD from inherited constructors (__cpp_deduction_guides >= 202207L)");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section03/inherited_ctor_ctad_cpp23", run>;

}  // namespace
