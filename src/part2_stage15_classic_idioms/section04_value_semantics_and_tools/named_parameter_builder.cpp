// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : named_parameter_builder
// Topic id : part2/stage15/section04/named_parameter_builder
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <chrono>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section04/named_parameter_builder";

class Request {
public:
    Request& url(std::string value) & {
        url_ = std::move(value);
        return *this;
    }
    Request& retries(int value) & {
        retries_ = value;
        return *this;
    }
    Request& timeout(std::chrono::milliseconds value) & {
        timeout_ = value;
        return *this;
    }

    const std::string& url() const noexcept { return url_; }
    int retries() const noexcept { return retries_; }
    std::chrono::milliseconds timeout() const noexcept { return timeout_; }

private:
    std::string url_;
    int retries_{3};
    std::chrono::milliseconds timeout_{1'000};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Request request;
    request.url("https://example.test").retries(5).timeout(std::chrono::milliseconds{250});
    LEARN_EXPECT_EQ(checks, request.url(), std::string{"https://example.test"});
    LEARN_EXPECT_EQ(checks, request.retries(), 5);
    LEARN_EXPECT_EQ(checks, request.timeout(), std::chrono::milliseconds{250});
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section04/named_parameter_builder", run>;

}  // namespace
