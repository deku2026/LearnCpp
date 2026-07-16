// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section04_customization_points
// Item     : tag_invoke_overview
// Topic id : part6/d/section04/tag_invoke_overview
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <concepts>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/d/section04/tag_invoke_overview";

namespace protocol {

void tag_invoke() = delete;  // ADL barrier; only associated-namespace overloads may match.

struct describe_t {
    template <class T>
        requires requires(describe_t tag, T&& value) { tag_invoke(tag, std::forward<T>(value)); }
    constexpr decltype(auto) operator()(T&& value) const noexcept(noexcept(tag_invoke(*this, std::forward<T>(value)))) {
        return tag_invoke(*this, std::forward<T>(value));
    }
};

inline constexpr describe_t describe{};

}  // namespace protocol

namespace model {

struct User {
    friend constexpr std::string_view tag_invoke(protocol::describe_t, const User&) noexcept { return "custom user"; }
};

struct Unregistered {};

}  // namespace model

static_assert(std::invocable<decltype(protocol::describe), const model::User&>);
static_assert(!std::invocable<decltype(protocol::describe), const model::Unregistered&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, protocol::describe(model::User{}), std::string_view{"custom user"});
    // tag_invoke is a library convention described by P1895, not a C++23
    // standard-library facility. The tag type distinguishes each protocol.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section04/tag_invoke_overview", run>;

}  // namespace
