// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : members_and_access_control
// Topic id : part2/stage04/section01/members_and_access_control
// References: [class.access], [class.mfct]

#include "learn/example_support.hpp"

#include <string>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section01/members_and_access_control";

class Account {
public:
    explicit Account(std::string owner) : owner_(std::move(owner)) {}

    [[nodiscard]] bool deposit(int amount) noexcept {
        if (amount <= 0) {
            return false;
        }
        balance_ += amount;
        return true;
    }

    [[nodiscard]] int balance() const noexcept { return balance_; }
    [[nodiscard]] const std::string& owner() const noexcept { return owner_; }

private:
    std::string owner_;
    int balance_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Account account{"Ada"};
    LEARN_EXPECT(checks, account.deposit(25));
    LEARN_EXPECT(checks, !account.deposit(0));
    LEARN_EXPECT_EQ(checks, account.balance(), 25);
    LEARN_EXPECT_EQ(checks, account.owner(), std::string{"Ada"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section01/members_and_access_control", run>;

}  // namespace
