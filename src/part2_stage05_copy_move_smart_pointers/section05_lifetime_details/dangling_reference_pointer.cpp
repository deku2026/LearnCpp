// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section05_lifetime_details
// Item     : dangling_reference_pointer
// Topic id : part2/stage05/section05/dangling_reference_pointer
// References: [basic.life], [dcl.ref]

#include "learn/example_support.hpp"

#include <memory>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section05/dangling_reference_pointer";

#if 0
int* dangling_pointer() {
    int local = 7;
    return &local;  // intentionally disabled: the pointee dies on return
}
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    std::weak_ptr<int> observer;
    {
        auto owner = std::make_shared<int>(7);
        observer = owner;
        if (auto borrowed = observer.lock()) {
            LEARN_EXPECT_EQ(checks, *borrowed, 7);
        }
    }
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT_EQ(checks, observer.lock(), std::shared_ptr<int>{});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section05/dangling_reference_pointer", run>;

}  // namespace
