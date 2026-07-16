// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section02_raii
// Item     : raii_resource_handle
// Topic id : part2/stage04/section02/raii_resource_handle
// References: [class.dtor], [class.copy.ctor], C++ Core Guidelines R.1

#include "learn/example_support.hpp"

#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section02/raii_resource_handle";

class UniqueHandle {
public:
    UniqueHandle() : value_(++next_), owns_(true) { ++open_; }
    ~UniqueHandle() { reset(); }

    UniqueHandle(const UniqueHandle&) = delete;
    UniqueHandle& operator=(const UniqueHandle&) = delete;

    UniqueHandle(UniqueHandle&& other) noexcept
        : value_(std::exchange(other.value_, 0)), owns_(std::exchange(other.owns_, false)) {}

    UniqueHandle& operator=(UniqueHandle&& other) noexcept {
        if (this != &other) {
            reset();
            value_ = std::exchange(other.value_, 0);
            owns_ = std::exchange(other.owns_, false);
        }
        return *this;
    }

    [[nodiscard]] int get() const noexcept { return value_; }
    [[nodiscard]] bool owns_resource() const noexcept { return owns_; }
    [[nodiscard]] static int open_count() noexcept { return open_; }

private:
    void reset() noexcept {
        if (owns_) {
            --open_;
            owns_ = false;
            value_ = 0;
        }
    }

    int value_{};
    bool owns_{};
    inline static int next_{};
    inline static int open_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LEARN_EXPECT_EQ(checks, UniqueHandle::open_count(), 0);
    {
        UniqueHandle first;
        const int raw = first.get();
        UniqueHandle second{std::move(first)};
        LEARN_EXPECT(checks, !first.owns_resource());
        LEARN_EXPECT(checks, second.owns_resource());
        LEARN_EXPECT_EQ(checks, second.get(), raw);
        LEARN_EXPECT_EQ(checks, UniqueHandle::open_count(), 1);
    }
    LEARN_EXPECT_EQ(checks, UniqueHandle::open_count(), 0);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section02/raii_resource_handle", run>;

}  // namespace
