// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : nsdmi_default_member_init
// Topic id : part2/stage04/section01/nsdmi_default_member_init
//
// Covers: NSDMI defaults, ctor init-list override, declaration-order init

#include "learn/topic_registry.hpp"

#include <cassert>
#include <string>

namespace {

class Config {
    int timeout_ = 30;
    bool verbose_ = false;
    std::string path_ = "/tmp";

public:
    Config() = default;
    explicit Config(int t) : timeout_(t) {}
    Config(int t, std::string path) : timeout_(t), path_(std::move(path)) {}

    int timeout() const { return timeout_; }
    bool verbose() const { return verbose_; }
    const std::string& path() const { return path_; }
};

class Ordered {
    int a_ = 1;
    int b_ = a_ + 10;

public:
    Ordered() = default;
    explicit Ordered(int a) : a_(a) {}

    int a() const { return a_; }
    int b() const { return b_; }
};

void demo_basics() {
    Config c;
    assert(c.timeout() == 30);
    assert(!c.verbose());
    assert(c.path() == "/tmp");
}

void demo_intermediate() {
    Config c{5};
    assert(c.timeout() == 5);
    assert(c.path() == "/tmp");

    Config d{9, "/var"};
    assert(d.timeout() == 9);
    assert(d.path() == "/var");
}

void demo_expert() {
    // Members initialize in declaration order; NSDMI applies when not listed.
    Ordered o;
    assert(o.a() == 1);
    assert(o.b() == 11);

    Ordered p{7};
    assert(p.a() == 7);
    // Declaration order: a_ first (from list), then b_ NSDMI sees a_ == 7.
    assert(p.b() == 17);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/nsdmi_default_member_init", run>;

}  // namespace
