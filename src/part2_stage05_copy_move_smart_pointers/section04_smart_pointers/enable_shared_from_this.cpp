// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : enable_shared_from_this
// Topic id : part2/stage05/section04/enable_shared_from_this
// References: [util.smartptr.enab]

#include "learn/example_support.hpp"

#include <memory>
#include <string>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/enable_shared_from_this";

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(std::string name) : name_(std::move(name)) {}
    [[nodiscard]] std::shared_ptr<Session> self() { return shared_from_this(); }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }

private:
    std::string name_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    auto owned = std::make_shared<Session>("owned");
    auto same = owned->self();
    LEARN_EXPECT_EQ(checks, same.get(), owned.get());
    LEARN_EXPECT_EQ(checks, owned.use_count(), 2L);

    Session stack{"stack"};
    LEARN_EXPECT_THROWS(checks, std::bad_weak_ptr, stack.self());

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/enable_shared_from_this", run>;

}  // namespace
