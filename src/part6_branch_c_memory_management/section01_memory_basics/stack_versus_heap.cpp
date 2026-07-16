// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : stack_versus_heap
// Topic id : part6/c/section01/stack_versus_heap
// Reference: C++23 working draft [basic.stc], [expr.new], [unique.ptr].

#include "learn/example_support.hpp"

#include <memory>
#include <string>
#include <vector>

namespace {

struct LifetimeProbe {
    int* live{};
    std::string label;

    LifetimeProbe(int& count, std::string text) : live(&count), label(std::move(text)) { ++*live; }
    ~LifetimeProbe() { --*live; }

    LifetimeProbe(const LifetimeProbe&) = delete;
    LifetimeProbe& operator=(const LifetimeProbe&) = delete;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section01/stack_versus_heap";
    learn::ExampleChecks checks{topic};
    int live{};

    {
        LifetimeProbe automatic{live, "automatic storage duration"};
        auto dynamic = std::make_unique<LifetimeProbe>(live, "dynamic storage duration");
        LEARN_EXPECT_EQ(checks, live, 2);
        LEARN_EXPECT_EQ(checks, automatic.label, std::string{"automatic storage duration"});

        dynamic.reset();
        LEARN_EXPECT_EQ(checks, live, 1);
    }
    LEARN_EXPECT_EQ(checks, live, 0);

    // Storage duration and ownership are separate ideas. vector owns dynamic storage while the
    // vector object itself has automatic storage duration and releases that storage by RAII.
    std::vector<int> values(64, 3);
    LEARN_EXPECT_EQ(checks, values.size(), std::size_t{64});
    LEARN_EXPECT_EQ(checks, values.front(), 3);

    // The standard does not promise a literal machine "stack" or "heap", nor any address order.
    // Prefer automatic objects and owning containers/smart pointers; use raw new only at a narrow
    // ownership boundary where the matching delete is mechanically guaranteed.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/stack_versus_heap", run>;

}  // namespace
