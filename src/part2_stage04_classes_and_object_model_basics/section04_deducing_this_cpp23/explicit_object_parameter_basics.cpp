// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : explicit_object_parameter_basics
// Topic id : part2/stage04/section04/explicit_object_parameter_basics
// References: [dcl.fct], P0847R7

#include "learn/example_support.hpp"

#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section04/explicit_object_parameter_basics";

class Box {
public:
    explicit Box(int value) : value_(value) {}

#if defined(__cpp_explicit_this_parameter) && __cpp_explicit_this_parameter >= 202110L
    template <class Self>
    decltype(auto) value(this Self&& self) noexcept {
        return (std::forward<Self>(self).value_);
    }
#else
    int& value() & noexcept { return value_; }
    const int& value() const& noexcept { return value_; }
#endif

private:
    int value_;
};

static_assert(std::is_same_v<decltype(std::declval<Box&>().value()), int&>);
static_assert(std::is_same_v<decltype(std::declval<const Box&>().value()), const int&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Box box{3};
    box.value() = 8;
    const Box& read_only = box;
    LEARN_EXPECT_EQ(checks, read_only.value(), 8);

    return checks.result();
}

[[maybe_unused]] const auto& registered =
    ::learn::topic<"part2/stage04/section04/explicit_object_parameter_basics", run>;

}  // namespace
