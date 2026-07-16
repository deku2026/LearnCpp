// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section04_coroutine_handle
// Item     : handle_scheduling_intro
// Topic id : part2/stage12/section04/handle_scheduling_intro

#include "learn/example_support.hpp"

#include <coroutine>
#include <deque>
#include <exception>
#include <string>
#include <utility>
#include <vector>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section04/handle_scheduling_intro";

class FifoScheduler {
public:
    void enqueue(std::coroutine_handle<> handle) { ready_.push_back(handle); }

    bool run_one() {
        if (ready_.empty()) {
            return false;
        }
        const auto next = ready_.front();
        ready_.pop_front();
        if (next && !next.done()) {
            next.resume();
        }
        return true;
    }

    void drain() {
        while (run_one()) {
        }
    }

    [[nodiscard]] bool empty() const noexcept { return ready_.empty(); }

private:
    std::deque<std::coroutine_handle<>> ready_;
};

struct ScheduleLater {
    FifoScheduler* scheduler;

    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> current) const { scheduler->enqueue(current); }
    void await_resume() const noexcept {}
};

class ScheduledTask {
public:
    struct promise_type {
        std::exception_ptr exception;
        ScheduledTask get_return_object() noexcept {
            return ScheduledTask{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit ScheduledTask(Handle handle) noexcept : handle_(handle) {}
    ~ScheduledTask() {
        if (handle_) {
            handle_.destroy();
        }
    }
    ScheduledTask(const ScheduledTask&) = delete;
    ScheduledTask& operator=(const ScheduledTask&) = delete;
    ScheduledTask(ScheduledTask&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    ScheduledTask& operator=(ScheduledTask&&) = delete;

    void schedule(FifoScheduler& scheduler) const { scheduler.enqueue(handle_); }
    [[nodiscard]] bool done() const noexcept { return handle_.done(); }
    void rethrow_if_exception() const {
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
    }

private:
    Handle handle_{};
};

ScheduledTask worker(std::string id, FifoScheduler& scheduler, std::vector<std::string>& log) {
    log.push_back(id + "1");
    co_await ScheduleLater{&scheduler};
    log.push_back(id + "2");
    co_await ScheduleLater{&scheduler};
    log.push_back(id + "3");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    FifoScheduler scheduler;
    std::vector<std::string> log;
    auto first = worker("A", scheduler, log);
    auto second = worker("B", scheduler, log);
    first.schedule(scheduler);
    second.schedule(scheduler);
    scheduler.drain();

    LEARN_EXPECT(checks, scheduler.empty());
    LEARN_EXPECT(checks, first.done());
    LEARN_EXPECT(checks, second.done());
    first.rethrow_if_exception();
    second.rethrow_if_exception();
    LEARN_EXPECT_EQ(checks, (log), (std::vector<std::string>{"A1", "B1", "A2", "B2", "A3", "B3"}));
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section04/handle_scheduling_intro", run>;

}  // namespace
