// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : deducing_this_restrictions
// Topic id : part2/stage04/section04/deducing_this_restrictions
// References: [dcl.fct], P0847R7

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section04/deducing_this_restrictions";

struct Inspector {
    int value{7};

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    template <class Self>
    [[nodiscard]] int inspect(this Self&& self) noexcept {
        return self.value;  // There is no implicit `this`; use the named object parameter.
    }
#else
    [[nodiscard]] int inspect() const noexcept { return value; }
#endif
};

template <class T>
concept Inspectable = requires(T&& value) { std::forward<T>(value).inspect(); };

static_assert(Inspectable<Inspector&>);
static_assert(Inspectable<const Inspector&>);

#if 0
// Explicit-object member functions cannot be static or virtual and have no cv/ref qualifier.
struct Invalid {
    virtual void f(this Invalid&);  // intentionally ill-formed teaching counterexample
};
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Inspector mutable_value;
    const Inspector const_value{11};
    LEARN_EXPECT_EQ(checks, mutable_value.inspect(), 7);
    LEARN_EXPECT_EQ(checks, const_value.inspect(), 11);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section04/deducing_this_restrictions", run>;

}  // namespace
