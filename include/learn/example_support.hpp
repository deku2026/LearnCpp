// Small, dependency-free checks shared by the runnable teaching examples.
//
// Unlike assert(), these checks remain active in Release builds.  A failed
// check reports the topic and source line, then lets the registry continue to
// the next topic.  Examples that teach undefined behaviour or compilation
// failures must keep the dangerous form inactive and verify a safe counterpart.
#pragma once

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <functional>
#include <source_location>
#include <string_view>
#include <utility>

namespace learn {

class ExampleChecks {
public:
    explicit constexpr ExampleChecks(std::string_view topic) noexcept : topic_(topic) {}

    void expect(bool condition, std::string_view expression,
                std::source_location where = std::source_location::current()) noexcept {
        ++checks_;
        if (condition) {
            return;
        }

        ++failures_;
        std::fprintf(stderr, "[FAIL] %.*s: %.*s (%s:%u)\n", static_cast<int>(topic_.size()), topic_.data(),
                     static_cast<int>(expression.size()), expression.data(), where.file_name(), where.line());
    }

    template <class Left, class Right>
    void expect_equal(const Left& left, const Right& right, std::string_view expression,
                      std::source_location where =
                          std::source_location::current()) noexcept(noexcept(std::equal_to<>{}(left, right))) {
        expect(std::equal_to<>{}(left, right), expression, where);
    }

    template <class Exception, class Function>
    void expect_throws(Function&& function, std::string_view expression,
                       std::source_location where = std::source_location::current()) noexcept {
        ++checks_;
        try {
            std::invoke(std::forward<Function>(function));
        } catch (const Exception&) {
            return;
        } catch (...) {
        }

        ++failures_;
        std::fprintf(stderr, "[FAIL] %.*s: expected exception from %.*s (%s:%u)\n", static_cast<int>(topic_.size()),
                     topic_.data(), static_cast<int>(expression.size()), expression.data(), where.file_name(),
                     where.line());
    }

    [[nodiscard]] constexpr int result() const noexcept { return failures_ == 0 ? 0 : 1; }
    [[nodiscard]] constexpr int checks() const noexcept { return checks_; }
    [[nodiscard]] constexpr int failures() const noexcept { return failures_; }

    [[nodiscard]] static int unavailable(std::string_view topic, std::string_view feature) noexcept {
        std::fprintf(stdout, "[SKIP] %.*s: %.*s is unavailable in this compiler/library combination\n",
                     static_cast<int>(topic.size()), topic.data(), static_cast<int>(feature.size()), feature.data());
        return 0;
    }

private:
    std::string_view topic_;
    int checks_{};
    int failures_{};
};

[[nodiscard]] inline std::string_view argument_or(int argc, char* const* argv, int index,
                                                  std::string_view fallback) noexcept {
    if (argv == nullptr || index < 0 || index >= argc || argv[index] == nullptr) {
        return fallback;
    }
    return argv[index];
}

}  // namespace learn

#define LEARN_EXPECT(checks, expression) \
    (checks).expect(static_cast<bool>(expression), #expression, std::source_location::current())

#define LEARN_EXPECT_EQ(checks, left, right) \
    (checks).expect_equal((left), (right), #left " == " #right, std::source_location::current())

#define LEARN_EXPECT_THROWS(checks, exception_type, expression)                                 \
    (checks).expect_throws<exception_type>([&] { static_cast<void>(expression); }, #expression, \
                                           std::source_location::current())
