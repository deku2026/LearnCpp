// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section03_const_constexpr_volatile
// Item     : volatile_is_not_thread_sync
// Topic id : part2/stage02/section03/volatile_is_not_thread_sync
//

#include "learn/example_support.hpp"

#include <atomic>
#include <type_traits>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    volatile int device_register = 0;
    device_register = 7;
    const int observed = device_register;

    std::atomic<int> synchronized{0};
    synchronized.store(7, std::memory_order_release);
    const int loaded = synchronized.load(std::memory_order_acquire);

    static_assert(std::is_volatile_v<decltype(device_register)>);
    static_assert(!std::is_same_v<volatile int, std::atomic<int>>);

    learn::ExampleChecks checks{"part2/stage02/section03/volatile_is_not_thread_sync"};
    LEARN_EXPECT_EQ(checks, observed, 7);
    LEARN_EXPECT_EQ(checks, loaded, 7);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/volatile_is_not_thread_sync", run>;

}  // namespace
