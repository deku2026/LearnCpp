// Runnable teaching example
// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section02_lock_free
// Item     : lock_free_queue_spsc
// Topic id : part6/f/section02/lock_free_queue_spsc
// References: C++23 [atomics.order], single-producer/single-consumer ring design

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/f/section02/lock_free_queue_spsc";

template <class T, std::size_t Capacity>
class SpscQueue {
    static_assert(Capacity >= 2, "one slot distinguishes full from empty");

    static constexpr std::size_t kCacheLine = 64;
    using SlotArray = std::array<std::optional<T>, Capacity>;

    struct alignas(kCacheLine) CacheLineIndex {
        std::atomic<std::size_t> value{0};
        std::array<std::byte, kCacheLine - sizeof(std::atomic<std::size_t>)> explicit_padding{};
    };

    static_assert(sizeof(std::atomic<std::size_t>) <= kCacheLine);
    static_assert(sizeof(CacheLineIndex) == kCacheLine);
    static constexpr std::size_t kSlotPadding = kCacheLine - (sizeof(SlotArray) % kCacheLine);

public:
    bool try_push(T&& value) {
        const std::size_t tail = tail_.value.load(std::memory_order_relaxed);  // producer-owned
        const std::size_t next = increment(tail);
        if (next == head_.value.load(std::memory_order_acquire)) {  // observe released slot
            return false;
        }
        slots_[tail].emplace(std::move(value));
        tail_.value.store(next, std::memory_order_release);  // publish constructed value
        return true;
    }

    bool try_pop(T& output) {
        const std::size_t head = head_.value.load(std::memory_order_relaxed);  // consumer-owned
        if (head == tail_.value.load(std::memory_order_acquire)) {             // acquire published value
            return false;
        }
        output = std::move(*slots_[head]);
        slots_[head].reset();
        head_.value.store(increment(head), std::memory_order_release);  // publish reusable slot
        return true;
    }

private:
    static constexpr std::size_t increment(std::size_t index) noexcept { return (index + 1) % Capacity; }

    SlotArray slots_{};
    std::array<std::byte, kSlotPadding> explicit_slot_padding_{};
    CacheLineIndex head_{};
    CacheLineIndex tail_{};
};

struct Message {
    int sequence{};
    std::string text;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    constexpr int message_count = 2'000;
    SpscQueue<Message, 32> queue;
    bool order_is_correct = true;
    long long sum{};

    std::thread producer{[&] {
        for (int sequence = 1; sequence <= message_count; ++sequence) {
            Message message{sequence, "message-" + std::to_string(sequence)};
            while (!queue.try_push(std::move(message))) {
                std::this_thread::yield();
            }
        }
    }};
    std::thread consumer{[&] {
        for (int expected = 1; expected <= message_count; ++expected) {
            Message message;
            while (!queue.try_pop(message)) {
                std::this_thread::yield();
            }
            order_is_correct = order_is_correct && message.sequence == expected;
            order_is_correct = order_is_correct && message.text == "message-" + std::to_string(expected);
            sum += message.sequence;
        }
    }};
    producer.join();
    consumer.join();

    LEARN_EXPECT(checks, order_is_correct);
    LEARN_EXPECT_EQ(checks, sum, static_cast<long long>(message_count) * (message_count + 1) / 2);
    Message extra;
    LEARN_EXPECT(checks, !queue.try_pop(extra));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/f/section02/lock_free_queue_spsc", run>;

}  // namespace
