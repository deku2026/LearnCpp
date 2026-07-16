// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : use_after_free_deep
// Topic id : part6/g/section03/use_after_free_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <memory>
#include <optional>

namespace {

struct Session {
    int id;
};

[[nodiscard]] std::optional<int> read_if_alive(const std::weak_ptr<Session>& observer) {
    if (const auto owner = observer.lock()) {
        return owner->id;
    }
    return std::nullopt;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/use_after_free_deep"};

    auto owner = std::make_shared<Session>(Session{42});
    std::weak_ptr<Session> observer = owner;
    LEARN_EXPECT_EQ(checks, read_if_alive(observer), std::optional<int>{42});
    owner.reset();
    LEARN_EXPECT(checks, observer.expired());
    LEARN_EXPECT(checks, !read_if_alive(observer).has_value());

#if 0
    Session* dangling = new Session{7};
    delete dangling;
    return dangling->id;
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/use_after_free_deep", run>;

}  // namespace
