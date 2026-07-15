// LearnCpp topic example
// Doc      : part6-branch-c-memory-management.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : new_delete_class_level
// Topic id : part6/c/section01/new_delete_class_level
//
// Covers: class-specific operator new/delete

#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <new>

namespace {

struct Tracked {
    int v;
    static int news;
    static int deletes;
    explicit Tracked(int x) : v(x) {}
    static void* operator new(std::size_t n) {
        ++news;
        return std::malloc(n);
    }
    static void operator delete(void* p) noexcept {
        ++deletes;
        std::free(p);
    }
};
int Tracked::news = 0;
int Tracked::deletes = 0;

void demo_basics() {
    Tracked::news = Tracked::deletes = 0;
    Tracked* p = new Tracked(1);
    LEARN_CHECK(p->v == 1);
    delete p;
    LEARN_CHECK(Tracked::news == 1);
    LEARN_CHECK(Tracked::deletes == 1);
}

void demo_intermediate() {
    Tracked stack{2};  // does not call operator new
    LEARN_CHECK(stack.v == 2);
}

void demo_expert() {
    Tracked::news = Tracked::deletes = 0;
    Tracked* p = new Tracked(3);
    delete p;
    LEARN_CHECK(Tracked::news == Tracked::deletes);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/new_delete_class_level", run>;

}  // namespace
