// Doc      : 第6部分-支线C-内存管理.md
// Stage    : part6_branch_c_memory_management
// Section  : section01_memory_basics
// Item     : placement_new_and_destroy
// Topic id : part6/c/section01/placement_new_and_destroy
// Reference: C++23 working draft [new.delete.placement], [specialized.construct].

#include "learn/example_support.hpp"

#include <cstddef>
#include <memory>
#include <new>
#include <string>

namespace {

struct Message {
    std::string text;
    int* destructions{};

    ~Message() { ++*destructions; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/c/section01/placement_new_and_destroy";
    learn::ExampleChecks checks{topic};
    alignas(Message) std::byte storage[sizeof(Message)];
    int destructions{};

    Message* first = std::construct_at(reinterpret_cast<Message*>(storage), "first", &destructions);
    LEARN_EXPECT_EQ(checks, first->text, std::string{"first"});
    std::destroy_at(first);
    LEARN_EXPECT_EQ(checks, destructions, 1);

    // The storage remains; a new lifetime can begin in it. Always use the pointer returned by the
    // construction operation (or launder an eligible old pointer when the rules require it).
    Message* second = ::new (static_cast<void*>(storage)) Message{"second", &destructions};
    LEARN_EXPECT_EQ(checks, second->text, std::string{"second"});
    std::destroy_at(second);
    LEARN_EXPECT_EQ(checks, destructions, 2);

#if 0
    Message* never_constructed = reinterpret_cast<Message*>(storage);
    (void)never_constructed->text;  // Undefined behavior: storage alone is not a live Message.
    std::destroy_at(second);         // Undefined behavior: the second lifetime already ended.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/c/section01/placement_new_and_destroy", run>;

}  // namespace
