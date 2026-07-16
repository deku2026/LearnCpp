// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : strong_typedef
// Topic id : part2/stage15/section04/strong_typedef
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <compare>
#include <concepts>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section04/strong_typedef";

template <class Tag, class Representation>
class Strong {
public:
    explicit constexpr Strong(Representation value) : value_(value) {}
    constexpr Representation value() const noexcept { return value_; }
    friend constexpr auto operator<=>(const Strong&, const Strong&) = default;

private:
    Representation value_;
};

struct UserIdTag;
struct ProductIdTag;
using UserId = Strong<UserIdTag, int>;
using ProductId = Strong<ProductIdTag, int>;

static_assert(!std::convertible_to<int, UserId>);
static_assert(!std::same_as<UserId, ProductId>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr UserId first{7};
    constexpr UserId second{8};
    static_assert(first < second);
    LEARN_EXPECT_EQ(checks, first.value(), 7);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section04/strong_typedef", run>;

}  // namespace
