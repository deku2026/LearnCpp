// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E6 type erasure)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : move_only_function_as_base_cpp23
// Topic id : part6/e/section06/move_only_function_as_base_cpp23
//
// 要点: std::move_only_function (C++23) 可包装只移可调用对象;
//       对比 std::function 要求 CopyConstructible。
// 参考: P0288; cppreference move_only_function

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#if defined(__cpp_lib_move_only_function) || (defined(_MSC_VER) && _MSC_VER >= 1930) || \
    (defined(__GNUC__) && __GNUC__ >= 12) ||                                            \
    (defined(__clang__) && defined(_LIBCPP_VERSION) && _LIBCPP_VERSION >= 170000)

#include <functional>  // move_only_function in <functional>

#define LEARN_HAS_MOF 1
#else
#define LEARN_HAS_MOF 0
#endif

namespace {

// 只移的可调用包装 (手写 fallback / 教学模型)
class MoveOnlyCallable {
    struct Concept {
        virtual ~Concept() = default;
        virtual int invoke(int) = 0;
    };
    template <typename F>
    struct Model : Concept {
        F f;
        explicit Model(F&& fn) : f(std::move(fn)) {}
        int invoke(int x) override { return f(x); }
    };
    std::unique_ptr<Concept> self_;

public:
    MoveOnlyCallable() = default;
    template <typename F>
    MoveOnlyCallable(F&& f) : self_(std::make_unique<Model<std::decay_t<F>>>(std::forward<F>(f))) {}

    MoveOnlyCallable(MoveOnlyCallable&&) noexcept = default;
    MoveOnlyCallable& operator=(MoveOnlyCallable&&) noexcept = default;
    MoveOnlyCallable(const MoveOnlyCallable&) = delete;
    MoveOnlyCallable& operator=(const MoveOnlyCallable&) = delete;

    explicit operator bool() const { return static_cast<bool>(self_); }
    int operator()(int x) const { return self_->invoke(x); }
};

struct UniqueAdder {
    std::unique_ptr<int> offset;
    explicit UniqueAdder(int n) : offset(std::make_unique<int>(n)) {}
    int operator()(int x) const { return x + *offset; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E6 move_only_function (C++23) ===\n";

    // std::function 不能持有 UniqueAdder (不可拷贝)
    // std::function<int(int)> bad = UniqueAdder{1}; // ill-formed

    MoveOnlyCallable hand{UniqueAdder{5}};
    assert(hand(10) == 15);

    MoveOnlyCallable moved = std::move(hand);
    assert(moved(1) == 6);
    assert(!hand);

#if LEARN_HAS_MOF
    std::move_only_function<int(int)> mof = UniqueAdder{100};
    assert(mof(3) == 103);
    std::move_only_function<int(int)> mof2 = std::move(mof);
    assert(mof2(0) == 100);
    std::cout << "  std::move_only_function available on this STL\n";
#else
    std::cout << "  std::move_only_function not in this STL;"
                 " demonstrated handwritten move-only erasure\n";
#endif

    // 可搭配 unique_ptr 捕获
    auto up = std::make_unique<std::string>("pay");
    MoveOnlyCallable pay{[p = std::move(up)](int n) { return n + static_cast<int>(p->size()); }};
    assert(pay(1) == 4);

    std::cout << "  use move_only_function for unique_ptr captures / move-only"
                 " callables; function still for copyable callbacks\n";
    std::cout << "move_only_function_as_base_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/move_only_function_as_base_cpp23", run>;

}  // namespace
