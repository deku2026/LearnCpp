// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : null_dereference_deep
// Topic id : part6/g/section03/null_dereference_deep

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <functional>
#include <optional>
#include <stdexcept>

namespace {

template <class T>
class NotNull {
public:
    explicit NotNull(T* pointer) : pointer_(pointer) {
        if (pointer_ == nullptr) {
            throw std::invalid_argument{"NotNull requires an object"};
        }
    }
    [[nodiscard]] T& operator*() const noexcept { return *pointer_; }
    [[nodiscard]] T* operator->() const noexcept { return pointer_; }

private:
    T* pointer_;
};

template <class T>
[[nodiscard]] std::optional<std::reference_wrapper<T>> observe(T* pointer) noexcept {
    if (pointer == nullptr) {
        return std::nullopt;
    }
    return std::ref(*pointer);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section03/null_dereference_deep"};

    int value = 42;
    NotNull<int> required{&value};
    LEARN_EXPECT_EQ(checks, *required, 42);
    LEARN_EXPECT_EQ(checks, observe(&value)->get(), 42);
    LEARN_EXPECT(checks, !observe(static_cast<int*>(nullptr)).has_value());
    LEARN_EXPECT_THROWS(checks, std::invalid_argument, NotNull<int>{static_cast<int*>(nullptr)});

#if 0
    int invalid = *static_cast<int*>(nullptr);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/null_dereference_deep", run>;

}  // namespace
