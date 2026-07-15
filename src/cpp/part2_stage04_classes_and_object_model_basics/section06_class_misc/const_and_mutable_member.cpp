// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : const_and_mutable_member
// Topic id : part2/stage04/section06/const_and_mutable_member
//
// Covers: const data members, mutable for logical-const caches

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

class Config {
    const int id_;
    int value_ = 0;

public:
    explicit Config(int id) : id_(id) {}
    int id() const { return id_; }
    void set_value(int v) { value_ = v; }
    int value() const { return value_; }
};

class Cache {
    mutable int hits_ = 0;
    int value_ = 0;

public:
    explicit Cache(int v) : value_(v) {}
    int get() const {
        ++hits_;
        return value_;
    }
    int hits() const { return hits_; }
};

void demo_basics() {
    Config c{7};
    assert(c.id() == 7);
    c.set_value(3);
    assert(c.value() == 3);
}

void demo_intermediate() {
    const Cache cache{10};
    assert(cache.get() == 10);
    assert(cache.get() == 10);
    assert(cache.hits() == 2);
}

void demo_expert() {
    // const objects cannot call non-const methods; mutable still updates.
    Cache cache{1};
    const Cache& r = cache;
    (void)r.get();
    assert(cache.hits() == 1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/const_and_mutable_member", run>;

}  // namespace
