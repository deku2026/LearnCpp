// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : hidden_friend
// Topic id : part6/d/section01/hidden_friend
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <compare>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/hidden_friend";

namespace math {

class Vector2 {
public:
    constexpr Vector2(int x, int y) : x_(x), y_(y) {}

    [[nodiscard]] constexpr int x() const noexcept { return x_; }
    [[nodiscard]] constexpr int y() const noexcept { return y_; }

    // These namespace-scope friends do not participate in ordinary lookup,
    // but ADL can find them through their Vector2 arguments.
    friend constexpr Vector2 operator+(Vector2 left, Vector2 right) noexcept {
        return {left.x_ + right.x_, left.y_ + right.y_};
    }
    friend constexpr bool operator==(Vector2, Vector2) noexcept = default;

private:
    int x_{};
    int y_{};
};

}  // namespace math

static_assert(math::Vector2{1, 2} + math::Vector2{3, 4} == math::Vector2{4, 6});

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const auto sum = math::Vector2{1, 2} + math::Vector2{3, 4};
    LEARN_EXPECT_EQ(checks, sum.x(), 4);
    LEARN_EXPECT_EQ(checks, sum.y(), 6);
    LEARN_EXPECT(checks, (sum == math::Vector2{4, 6}));

#if 0
    // Ill-formed: the hidden friend has no namespace-scope declaration that
    // qualified lookup can see.
    auto bad = math::operator+(math::Vector2{1, 2}, math::Vector2{3, 4});
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/hidden_friend", run>;

}  // namespace
