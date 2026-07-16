// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : weak_ptr_break_cycles
// Topic id : part2/stage05/section04/weak_ptr_break_cycles
// References: [util.smartptr.weak]

#include "learn/example_support.hpp"

#include <memory>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/weak_ptr_break_cycles";

struct Parent;

struct Child {
    std::weak_ptr<Parent> parent;
    ~Child() { ++destroyed_children; }
    inline static int destroyed_children{};
};

struct Parent {
    std::shared_ptr<Child> child;
    ~Parent() { ++destroyed_parents; }
    inline static int destroyed_parents{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Parent::destroyed_parents = 0;
    Child::destroyed_children = 0;
    std::weak_ptr<Parent> observer;
    {
        auto parent = std::make_shared<Parent>();
        parent->child = std::make_shared<Child>();
        parent->child->parent = parent;
        observer = parent;
        LEARN_EXPECT(checks, !observer.expired());
    }
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT_EQ(checks, Parent::destroyed_parents, 1);
    LEARN_EXPECT_EQ(checks, Child::destroyed_children, 1);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/weak_ptr_break_cycles", run>;

}  // namespace
