// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : cpo_customization_point_object
// Topic id : part6/d/section04/cpo_customization_point_object
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/d/section04/cpo_customization_point_object";

namespace rendering {

namespace detail {
void render() = delete;  // Prevent unrelated ordinary-lookup fallbacks.

template <class T>
concept adl_renderable = requires(T&& value) { render(std::forward<T>(value)); };

struct render_fn {
    template <class T>
        requires adl_renderable<T>
    constexpr decltype(auto) operator()(T&& value) const noexcept(noexcept(render(std::forward<T>(value)))) {
        return render(std::forward<T>(value));
    }

    template <class T>
        requires(!adl_renderable<T> && requires(T&& value) { std::forward<T>(value).render(); })
    constexpr decltype(auto) operator()(T&& value) const noexcept(noexcept(std::forward<T>(value).render())) {
        return std::forward<T>(value).render();
    }
};

}  // namespace detail

inline constexpr detail::render_fn render{};

}  // namespace rendering

namespace model {

struct AdlWidget {
    friend constexpr std::string_view render(const AdlWidget&) noexcept { return "ADL"; }
};

struct MemberWidget {
    [[nodiscard]] constexpr std::string_view render() const noexcept { return "member"; }
};

struct Unsupported {};

}  // namespace model

static_assert(std::invocable<decltype(rendering::render), const model::AdlWidget&>);
static_assert(std::invocable<decltype(rendering::render), const model::MemberWidget&>);
static_assert(!std::invocable<decltype(rendering::render), const model::Unsupported&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    LEARN_EXPECT_EQ(checks, rendering::render(model::AdlWidget{}), std::string_view{"ADL"});
    LEARN_EXPECT_EQ(checks, rendering::render(model::MemberWidget{}), std::string_view{"member"});

    // A CPO is a constrained object: callers cannot specialize it as a
    // function template, and all lookup policy stays in one implementation.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section04/cpo_customization_point_object", run>;

}  // namespace
