// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : constructor_delegation
// Topic id : part2/stage04/section01/constructor_delegation
// References: [class.base.init], N1986

#include "learn/example_support.hpp"

#include <string>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section01/constructor_delegation";

class ConnectionOptions {
public:
    ConnectionOptions() : ConnectionOptions("localhost", 443) {}
    explicit ConnectionOptions(std::string host) : ConnectionOptions(std::move(host), 443) {}
    ConnectionOptions(std::string host, int port) : host_(std::move(host)), port_(port), secure_(port == 443) {}

    [[nodiscard]] const std::string& host() const noexcept { return host_; }
    [[nodiscard]] int port() const noexcept { return port_; }
    [[nodiscard]] bool secure() const noexcept { return secure_; }

private:
    std::string host_;
    int port_;
    bool secure_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    ConnectionOptions defaults;
    ConnectionOptions named{"example.test"};
    ConnectionOptions custom{"localhost", 8080};

    LEARN_EXPECT_EQ(checks, defaults.port(), 443);
    LEARN_EXPECT_EQ(checks, named.host(), std::string{"example.test"});
    LEARN_EXPECT(checks, named.secure());
    LEARN_EXPECT(checks, !custom.secure());

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section01/constructor_delegation", run>;

}  // namespace
