// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : destructor
// Topic id : part2/stage04/section03/destructor
// References: [class.dtor]

#include "learn/example_support.hpp"

#include <string>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/destructor";

class ScopeEvent {
public:
    ScopeEvent(std::vector<std::string>& log, std::string name) : log_(log), name_(std::move(name)) {
        log_.push_back("enter " + name_);
    }
    ~ScopeEvent() { log_.push_back("leave " + name_); }

private:
    std::vector<std::string>& log_;
    std::string name_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::vector<std::string> log;
    {
        ScopeEvent event{log, "scope"};
        LEARN_EXPECT_EQ(checks, log.back(), std::string{"enter scope"});
    }
    LEARN_EXPECT_EQ(checks, log.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, log.back(), std::string{"leave scope"});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/destructor", run>;

}  // namespace
