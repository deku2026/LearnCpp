// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section06_class_misc
// Item     : friend_function_class
// Topic id : part2/stage04/section06/friend_function_class
//
// Covers: friend free function and friend class access to private members

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

class Vault {
    int secret_ = 42;

    friend class Auditor;
    friend int inspect(const Vault&);
};

int inspect(const Vault& v) {
    return v.secret_;
}

class Auditor {
public:
    static int peek(const Vault& v) { return v.secret_; }
    static void set(Vault& v, int n) { v.secret_ = n; }
};

void demo_basics() {
    Vault v;
    assert(inspect(v) == 42);
}

void demo_intermediate() {
    Vault v;
    assert(Auditor::peek(v) == 42);
    Auditor::set(v, 7);
    assert(inspect(v) == 7);
}

void demo_expert() {
    // Friendship is not inherited or transitive.
    class Stranger {
    public:
        // cannot access Vault::secret_
        static int via_friend(const Vault& v) { return inspect(v); }
    };
    Vault v;
    assert(Stranger::via_friend(v) == 42);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section06/friend_function_class", run>;

}  // namespace
