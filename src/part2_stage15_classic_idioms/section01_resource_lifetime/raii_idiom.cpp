// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : raii_idiom
// Topic id : part2/stage15/section01/raii_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <stdexcept>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section01/raii_idiom";

class Resource {
public:
    explicit Resource(int& count) : count_(&count) { ++*count_; }
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    ~Resource() { --*count_; }

private:
    int* count_;
};

void operation(int& live, bool fail) {
    Resource resource{live};
    if (fail) throw std::runtime_error{"operation failed"};
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    int live{};
    operation(live, false);
    LEARN_EXPECT_EQ(checks, live, 0);
    LEARN_EXPECT_THROWS(checks, std::runtime_error, operation(live, true));
    LEARN_EXPECT_EQ(checks, live, 0);  // Stack unwinding invoked the destructor.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section01/raii_idiom", run>;

}  // namespace
