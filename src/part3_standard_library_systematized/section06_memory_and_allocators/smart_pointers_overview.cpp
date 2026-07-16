// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : smart_pointers_overview
// Topic id : part3/section06/smart_pointers_overview
// References: C++23 [smartptr], [unique.ptr], [util.smartptr.shared]

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section06/smart_pointers_overview";

struct CountingDelete {
    int* deletion_count{};

    void operator()(int* pointer) const noexcept {
        ++*deletion_count;
        delete pointer;
    }
};

struct Bundle {
    int id{7};
    int value{42};
};

struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> previous;
    int* destructions{};

    ~Node() { ++*destructions; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int deletions{};
    {
        std::unique_ptr<int, CountingDelete> owner{new int{11}, CountingDelete{&deletions}};
        LEARN_EXPECT_EQ(checks, *owner, 11);
        std::unique_ptr<int, CountingDelete> moved = std::move(owner);
        LEARN_EXPECT(checks, owner == nullptr);
        LEARN_EXPECT_EQ(checks, *moved, 11);
        moved.reset(new int{13});  // deletes 11 before taking the replacement
        LEARN_EXPECT_EQ(checks, deletions, 1);
        LEARN_EXPECT_EQ(checks, *moved, 13);
    }
    LEARN_EXPECT_EQ(checks, deletions, 2);

    // The aliasing constructor shares Bundle ownership while pointing at one
    // of its subobjects. The control block, not get(), identifies ownership.
    auto bundle = std::make_shared<Bundle>();
    std::shared_ptr<int> alias{bundle, &bundle->value};
    std::weak_ptr<int> observer = alias;
    LEARN_EXPECT_EQ(checks, bundle.use_count(), 2L);
    bundle.reset();
    LEARN_EXPECT_EQ(checks, *alias, 42);
    LEARN_EXPECT_EQ(checks, alias.use_count(), 1L);
    LEARN_EXPECT(checks, !observer.expired());
    alias.reset();
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT(checks, observer.lock() == nullptr);

    // One owning edge and one weak back-edge prevent a reference-count cycle.
    int destructions{};
    {
        auto first = std::make_shared<Node>();
        auto second = std::make_shared<Node>();
        first->destructions = &destructions;
        second->destructions = &destructions;
        first->next = second;
        second->previous = first;
        LEARN_EXPECT_EQ(checks, second->previous.lock(), first);
    }
    LEARN_EXPECT_EQ(checks, destructions, 2);

    auto array = std::make_unique<int[]>(3);
    array[0] = 2;
    array[1] = 3;
    array[2] = 5;
    LEARN_EXPECT_EQ(checks, array[0] + array[1] + array[2], 10);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/smart_pointers_overview", run>;

}  // namespace
