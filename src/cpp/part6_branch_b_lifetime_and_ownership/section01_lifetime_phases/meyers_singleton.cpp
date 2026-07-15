// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : meyers_singleton
// Topic id : part6/b/section01/meyers_singleton
//
// Covers: Meyers singleton via function-local static

#include "learn/topic_registry.hpp"

#include <string>

namespace {

class Config {
public:
    static Config& instance() {
        static Config cfg;
        return cfg;
    }
    int value = 0;
    std::string name = "default";

private:
    Config() = default;
};

void demo_basics() {
    Config::instance().value = 7;
    LEARN_CHECK(Config::instance().value == 7);
}

void demo_intermediate() {
    Config& a = Config::instance();
    Config& b = Config::instance();
    LEARN_CHECK(&a == &b);
    a.name = "app";
    LEARN_CHECK(b.name == "app");
}

void demo_expert() {
    // C++11: concurrent first entry is synchronized; later access is just a reference.
    Config::instance().value = 1;
    LEARN_CHECK(Config::instance().value == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/meyers_singleton", run>;

}  // namespace
