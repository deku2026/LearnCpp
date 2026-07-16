// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : aba_problem
// Topic id : part6/f/section02/aba_problem
// References: C++23 [atomics.types.operations.req]; safe-reclamation design notes

#include "learn/example_support.hpp"

#include <atomic>
#include <cstdint>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/f/section02/aba_problem";

constexpr std::uint64_t pack(std::uint32_t index, std::uint32_t version) noexcept {
    return (static_cast<std::uint64_t>(version) << 32U) | index;
}

constexpr std::uint32_t unpack_index(std::uint64_t value) noexcept {
    return static_cast<std::uint32_t>(value & 0xFFFF'FFFFULL);
}

constexpr std::uint32_t unpack_version(std::uint64_t value) noexcept {
    return static_cast<std::uint32_t>(value >> 32U);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    // A deterministic interleaving model: the plain index returns 1 -> 2 -> 1,
    // so a stale CAS cannot tell that two modifications happened in between.
    std::atomic<std::uint32_t> naive_head{1};
    std::uint32_t stale_index = naive_head.load();
    naive_head.store(2);
    naive_head.store(1);
    const bool naive_cas_succeeded = naive_head.compare_exchange_strong(stale_index, 3);
    LEARN_EXPECT(checks, naive_cas_succeeded);
    LEARN_EXPECT_EQ(checks, naive_head.load(), 3U);

    // Pairing the logical index with a monotonically changing tag turns ABA
    // into (1,0) -> (2,1) -> (1,2), which no longer compares equal.
    std::atomic<std::uint64_t> tagged_head{pack(1, 0)};
    const std::uint64_t stale_tagged = tagged_head.load();
    tagged_head.store(pack(2, 1));
    tagged_head.store(pack(1, 2));
    std::uint64_t expected = stale_tagged;
    const bool tagged_cas_succeeded = tagged_head.compare_exchange_strong(expected, pack(3, 1));
    LEARN_EXPECT(checks, !tagged_cas_succeeded);
    LEARN_EXPECT_EQ(checks, unpack_index(expected), 1U);
    LEARN_EXPECT_EQ(checks, unpack_version(expected), 2U);

    // A finite tag can eventually wrap, and no tag by itself makes reclamation
    // safe. Production pointer structures also need hazard pointers, epochs/RCU,
    // reference counting, or another proof that a node cannot be freed while observed.
#if 0
    // Inactive dangerous form: another thread may still dereference old_head.
    Node* old_head = head.load(std::memory_order_acquire);
    if (head.compare_exchange_strong(old_head, old_head->next)) {
        delete old_head; // possible use-after-free without safe reclamation
    }
#endif
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section02/aba_problem", run>;

}  // namespace
