// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : nvi_non_virtual_interface
// Topic id : part2/stage15/section02/nvi_non_virtual_interface
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <memory>
#include <stdexcept>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section02/nvi_non_virtual_interface";

class Processor {
public:
    virtual ~Processor() = default;
    int process(int input) {
        if (input < 0) throw std::invalid_argument{"negative input"};
        const int result = do_process(input);
        if (result < 0) throw std::logic_error{"invalid implementation result"};
        return result;
    }

private:
    virtual int do_process(int input) = 0;
};

class Doubler final : public Processor {
private:
    int do_process(int input) override { return input * 2; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    std::unique_ptr<Processor> processor = std::make_unique<Doubler>();
    LEARN_EXPECT_EQ(checks, processor->process(4), 8);
    LEARN_EXPECT_THROWS(checks, std::invalid_argument, processor->process(-1));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section02/nvi_non_virtual_interface", run>;

}  // namespace
