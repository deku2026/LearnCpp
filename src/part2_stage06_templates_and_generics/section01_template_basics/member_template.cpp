// A member template has template parameters independent of its enclosing class.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section01_template_basics
// Item     : member_template
// Topic id : part2/stage06/section01/member_template
//
// NumberBox<double> can be constructed from and converted to several compatible types.

#include "learn/example_support.hpp"

#include <concepts>
#include <type_traits>
#include <utility>

namespace {

template <class T>
class NumberBox {
public:
    template <class U>
        requires std::convertible_to<U, T>
    explicit constexpr NumberBox(U&& value) : value_(std::forward<U>(value)) {}

    template <class U>
        requires std::convertible_to<T, U>
    [[nodiscard]] constexpr U as() const {
        return static_cast<U>(value_);
    }

private:
    T value_;
};

static_assert(std::is_constructible_v<NumberBox<double>, int>);
static_assert(NumberBox<double>{3}.as<int>() == 3);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section01/member_template"};
    const NumberBox<double> box{7};
    LEARN_EXPECT_EQ(checks, box.as<double>(), 7.0);
    LEARN_EXPECT_EQ(checks, box.as<int>(), 7);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section01/member_template", run>;

}  // namespace
