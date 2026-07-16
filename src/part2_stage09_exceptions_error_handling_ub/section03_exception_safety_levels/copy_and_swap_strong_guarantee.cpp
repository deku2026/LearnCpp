// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : copy_and_swap_strong_guarantee
// Topic id : part2/stage09/section03/copy_and_swap_strong_guarantee
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section03/copy_and_swap_strong_guarantee";

class Profile {
public:
    Profile(std::string name, int quota) : name_(std::move(name)), quota_(quota) {}
    Profile(const Profile&) = default;

    Profile& operator=(Profile replacement) noexcept {
        swap(*this, replacement);
        return *this;
    }

    friend void swap(Profile& left, Profile& right) noexcept {
        using std::swap;
        swap(left.name_, right.name_);
        swap(left.quota_, right.quota_);
    }

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] int quota() const noexcept { return quota_; }

private:
    std::string name_;
    int quota_;
};

Profile load_profile(bool fail) {
    if (fail) {
        throw std::runtime_error{"load failed before assignment"};
    }
    return {"new", 20};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Profile profile{"old", 5};
    LEARN_EXPECT_THROWS(checks, std::runtime_error, profile = load_profile(true));
    LEARN_EXPECT_EQ(checks, profile.name(), std::string{"old"});
    profile = load_profile(false);
    LEARN_EXPECT_EQ(checks, profile.quota(), 20);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section03/copy_and_swap_strong_guarantee", run>;

}  // namespace
