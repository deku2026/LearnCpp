// Runnable teaching example
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_error_code_category
// Topic id : part2/stage09/section05/std_error_code_category
// References: C++23 [except], [except.spec], [expected], [basic.life], [intro.abstract]

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <system_error>

namespace {

constexpr std::string_view kTopic = "part2/stage09/section05/std_error_code_category";

enum class ConfigError { missing_key = 1, invalid_value = 2 };

class ConfigCategory final : public std::error_category {
public:
    [[nodiscard]] const char* name() const noexcept override { return "learn.config"; }
    [[nodiscard]] std::string message(int value) const override {
        switch (static_cast<ConfigError>(value)) {
            case ConfigError::missing_key:
                return "missing key";
            case ConfigError::invalid_value:
                return "invalid value";
        }
        return "unknown config error";
    }
};

const std::error_category& config_category() {
    static const ConfigCategory category;
    return category;
}

std::error_code make_config_error(ConfigError error) {
    return {static_cast<int>(error), config_category()};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::error_code error = make_config_error(ConfigError::missing_key);
    LEARN_EXPECT(checks, static_cast<bool>(error));
    LEARN_EXPECT_EQ(checks, std::string{error.category().name()}, std::string{"learn.config"});
    LEARN_EXPECT_EQ(checks, error.message(), std::string{"missing key"});
    LEARN_EXPECT(checks, error != std::make_error_code(std::errc::no_such_file_or_directory));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage09/section05/std_error_code_category", run>;

}  // namespace
