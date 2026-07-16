// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_shared_single_allocation
// Topic id : part2/stage05/section04/make_shared_single_allocation
// References: [util.smartptr.shared.create]

#include "learn/example_support.hpp"

#include <memory>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/make_shared_single_allocation";

struct TrackedObject {
    explicit TrackedObject(int input) : value(input) { ++alive; }
    ~TrackedObject() { --alive; }
    int value;
    inline static int alive{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::weak_ptr<TrackedObject> observer;
    {
        auto owner = std::make_shared<TrackedObject>(17);
        observer = owner;
        LEARN_EXPECT_EQ(checks, TrackedObject::alive, 1);
        LEARN_EXPECT_EQ(checks, owner->value, 17);
        LEARN_EXPECT_EQ(checks, owner.use_count(), 1L);
    }
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT_EQ(checks, TrackedObject::alive, 0);
    // make_shared permits the implementation to coallocate object and control block;
    // code must depend on ownership semantics, not a measured allocation count.

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/make_shared_single_allocation", run>;

}  // namespace
