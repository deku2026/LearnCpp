// Doc      : 第6部分-支线F-内存模型与并发.md
// Stage    : part6_branch_f_memory_model_and_concurrency
// Section  : section05_special_topics
// Item     : double_checked_locking
// Topic id : part6/f/section05/double_checked_locking
// Reference: C++23 working draft [atomics.order], [thread.mutex.requirements].

#include "learn/example_support.hpp"

#include <array>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace {

struct Configuration {
    int port;
    std::string protocol;
};

class LazyConfiguration {
public:
    const Configuration& get() {
        Configuration* value = published_.load(std::memory_order_acquire);
        if (value == nullptr) {
            const std::lock_guard lock{mutex_};
            value = published_.load(std::memory_order_acquire);
            if (value == nullptr) {
                auto created = std::make_unique<Configuration>(Configuration{443, "https"});
                value = created.get();
                owner_ = std::move(created);
                constructions_.fetch_add(1, std::memory_order_relaxed);
                published_.store(value, std::memory_order_release);
            }
        }
        return *value;
    }

    [[nodiscard]] int constructions() const noexcept { return constructions_.load(std::memory_order_relaxed); }

private:
    std::atomic<Configuration*> published_{nullptr};
    std::mutex mutex_;
    std::unique_ptr<Configuration> owner_;
    std::atomic<int> constructions_{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part6/f/section05/double_checked_locking";
    learn::ExampleChecks checks{topic};
    LazyConfiguration lazy;
    std::array<const Configuration*, 8> results{};
    std::vector<std::thread> threads;
    for (std::size_t index = 0; index < results.size(); ++index) {
        threads.emplace_back([&, index] { results[index] = &lazy.get(); });
    }
    for (std::thread& thread : threads) {
        thread.join();
    }

    for (const Configuration* result : results) {
        LEARN_EXPECT_EQ(checks, result, results.front());
        LEARN_EXPECT_EQ(checks, result->port, 443);
        LEARN_EXPECT_EQ(checks, result->protocol, std::string{"https"});
    }
    LEARN_EXPECT_EQ(checks, lazy.constructions(), 1);

    // Release publishes the fully initialized object; acquire makes its ordinary fields visible.
    // The mutex owns initialization and the unique_ptr owns lifetime. For most singletons, a local
    // static or call_once is simpler and less error-prone than hand-written DCLP.
#if 0
    volatile Configuration* broken = nullptr;  // volatile neither makes access atomic nor publishes fields.
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/f/section05/double_checked_locking", run>;

}  // namespace
