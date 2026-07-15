// LearnCpp topic example
// Doc      : part6-branch-b-lifetime-and-ownership.md
// Stage    : part6_branch_b_lifetime_and_ownership
// Section  : section01_lifetime_phases
// Item     : destruction_order
// Topic id : part6/b/section01/destruction_order
//
// Covers: reverse construction order for locals; static destruction reverse of completion

#include "learn/topic_registry.hpp"

#include <vector>

namespace {

std::vector<int>* g_log = nullptr;

struct Trace {
    int id;
    explicit Trace(int i) : id(i) {
        if (g_log) g_log->push_back(id);
    }
    ~Trace() {
        if (g_log) g_log->push_back(-id);
    }
};

void demo_basics() {
    std::vector<int> log;
    g_log = &log;
    {
        Trace a{1};
        Trace b{2};
    }
    g_log = nullptr;
    // construction 1,2 then destruction 2,1
    LEARN_CHECK(log.size() == 4);
    LEARN_CHECK(log[0] == 1 && log[1] == 2);
    LEARN_CHECK(log[2] == -2 && log[3] == -1);
}

void demo_intermediate() {
    std::vector<int> log;
    g_log = &log;
    {
        Trace outer{10};
        {
            Trace inner{20};
        }
        LEARN_CHECK(log.size() == 3);  // 10,20,-20
    }
    g_log = nullptr;
    LEARN_CHECK(log.back() == -10);
}

void demo_expert() {
    // Array elements: construct ascending index, destroy descending.
    std::vector<int> log;
    g_log = &log;
    {
        Trace arr[2]{Trace{1}, Trace{2}};
        (void)arr;
    }
    g_log = nullptr;
    LEARN_CHECK(log[0] == 1 && log[1] == 2);
    LEARN_CHECK(log[2] == -2 && log[3] == -1);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/b/section01/destruction_order", run>;

}  // namespace
