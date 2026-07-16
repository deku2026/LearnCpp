// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_default_noexcept
// Topic id : part2/stage09/section07/destructor_default_noexcept
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section07/destructor_default_noexcept";

struct SafeMember {
    ~SafeMember() = default;
};

struct RiskyMember {
    ~RiskyMember() noexcept(false) {}
};

struct SafeOwner {
    SafeMember member;
};

struct RiskyOwner {
    RiskyMember member;
};

static_assert(std::is_nothrow_destructible_v<SafeOwner>);
static_assert(!std::is_nothrow_destructible_v<RiskyOwner>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    {
        SafeOwner owner{};
        (void)owner;
    }
    LEARN_EXPECT(checks, noexcept(std::declval<SafeOwner&>().~SafeOwner()));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section07/destructor_default_noexcept", run>;

}  // namespace
