// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : small_buffer_optimization_in_erasure
// Topic id : part6/e/section06/small_buffer_optimization_in_erasure

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace {

class SboFunction {
private:
    static constexpr std::size_t capacity = 32;
    struct Operations {
        int (*call)(void*, int);
        void (*destroy)(void*, bool) noexcept;
    };

    template <class F>
    static constexpr Operations operations{[](void* object, int value) { return (*static_cast<F*>(object))(value); },
                                           [](void* object, bool heap) noexcept {
                                               if (heap) {
                                                   delete static_cast<F*>(object);
                                               } else {
                                                   std::destroy_at(static_cast<F*>(object));
                                               }
                                           }};

    template <class F>
    static constexpr bool fits_inline =
        sizeof(F) <= capacity && alignof(F) <= alignof(std::max_align_t) && std::is_nothrow_move_constructible_v<F>;

public:
    template <class F>
    explicit SboFunction(F function) : operations_(&operations<F>) {
        if constexpr (fits_inline<F>) {
            object_ = std::construct_at(reinterpret_cast<F*>(storage_.data()), std::move(function));
        } else {
            object_ = new F(std::move(function));
            heap_ = true;
        }
    }
    ~SboFunction() { operations_->destroy(object_, heap_); }
    SboFunction(const SboFunction&) = delete;
    SboFunction& operator=(const SboFunction&) = delete;
    SboFunction(SboFunction&&) = delete;
    SboFunction& operator=(SboFunction&&) = delete;

    [[nodiscard]] int operator()(int value) { return operations_->call(object_, value); }
    [[nodiscard]] bool uses_heap() const noexcept { return heap_; }

private:
    alignas(std::max_align_t) std::array<std::byte, capacity> storage_{};
    void* object_{};
    const Operations* operations_{};
    bool heap_{};
};

struct SmallCallable {
    int bias;
    [[nodiscard]] int operator()(int value) const noexcept { return value + bias; }
};

struct LargeCallable {
    std::array<int, 64> values{};
    [[nodiscard]] int operator()(int value) const noexcept { return value + values[0]; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section06/small_buffer_optimization_in_erasure"};

    SboFunction small{SmallCallable{2}};
    LEARN_EXPECT(checks, !small.uses_heap());
    LEARN_EXPECT_EQ(checks, small(40), 42);

    LargeCallable large_data;
    large_data.values[0] = 2;
    SboFunction large{large_data};
    LEARN_EXPECT(checks, large.uses_heap());
    LEARN_EXPECT_EQ(checks, large(40), 42);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section06/small_buffer_optimization_in_erasure", run>;

}  // namespace
