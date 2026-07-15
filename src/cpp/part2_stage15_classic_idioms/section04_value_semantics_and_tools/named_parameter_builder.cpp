// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section04_value_semantics_and_tools
// Item     : named_parameter_builder
// Topic id : part2/stage15/section04/named_parameter_builder
//
// Covers: named parameter / builder idiom

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

struct Config {
    int width = 0;
    int height = 0;
    std::string name;
};

class ConfigBuilder {
    Config c_;

public:
    ConfigBuilder& width(int w) {
        c_.width = w;
        return *this;
    }
    ConfigBuilder& height(int h) {
        c_.height = h;
        return *this;
    }
    ConfigBuilder& name(std::string n) {
        c_.name = std::move(n);
        return *this;
    }
    Config build() const { return c_; }
};

void demo_basics() {
    Config c = ConfigBuilder{}.width(10).height(20).build();
    assert(c.width == 10 && c.height == 20);
}

void demo_intermediate() {
    Config c = ConfigBuilder{}.name("ui").width(8).build();
    assert(c.name == "ui" && c.width == 8);
}

void demo_expert() {
    auto b = ConfigBuilder{}.width(1).height(2).name("x");
    Config c1 = b.build();
    Config c2 = b.width(9).build();
    assert(c1.width == 1);
    assert(c2.width == 9);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section04/named_parameter_builder", run>;

}  // namespace
