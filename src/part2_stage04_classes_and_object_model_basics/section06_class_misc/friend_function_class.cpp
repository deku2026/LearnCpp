// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : friend_function_class
// Topic id : part2/stage04/section06/friend_function_class
// References: [class.friend]

#include "learn/example_support.hpp"

#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section06/friend_function_class";

class Vault {
public:
    explicit Vault(int secret) : secret_(secret) {}

    friend int reveal(const Vault& vault) noexcept;
    friend class Auditor;

private:
    int secret_;
};

int reveal(const Vault& vault) noexcept {
    return vault.secret_;
}

class Auditor {
public:
    [[nodiscard]] bool matches(const Vault& vault, int expected) const noexcept { return vault.secret_ == expected; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    const Vault vault{73};
    const Auditor auditor;
    LEARN_EXPECT_EQ(checks, reveal(vault), 73);
    LEARN_EXPECT(checks, auditor.matches(vault, 73));
    LEARN_EXPECT(checks, !auditor.matches(vault, 0));

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section06/friend_function_class", run>;

}  // namespace
