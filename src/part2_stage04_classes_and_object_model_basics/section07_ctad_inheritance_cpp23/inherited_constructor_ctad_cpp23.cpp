// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section07_ctad_inheritance_cpp23
// Item     : inherited_constructor_ctad_cpp23
// Topic id : part2/stage04/section07/inherited_constructor_ctad_cpp23
// References: [over.match.class.deduct], P2582R1, SD-FeatureTest __cpp_deduction_guides

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section07/inherited_constructor_ctad_cpp23";

template <class T>
struct ValueBase {
    explicit ValueBase(T input) : value(std::move(input)) {}
    T value;
};

template <class T>
struct InheritingBox : ValueBase<T> {
    using ValueBase<T>::ValueBase;
};

#if !defined(__cpp_deduction_guides) || __cpp_deduction_guides < 202207L
template <class T>
InheritingBox(T) -> InheritingBox<T>;  // Portable fallback for pre-P2582 implementations.
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    InheritingBox integer_box{42};
    InheritingBox text_box{"text"};
    static_assert(std::is_same_v<decltype(integer_box), InheritingBox<int>>);
    static_assert(std::is_same_v<decltype(text_box), InheritingBox<const char*>>);
    LEARN_EXPECT_EQ(checks, integer_box.value, 42);
    LEARN_EXPECT_EQ(checks, std::string_view{text_box.value}, std::string_view{"text"});

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section07/inherited_constructor_ctad_cpp23", run>;

}  // namespace
