// LearnCpp example
// Doc      : 第2部分-阶段12-协程.md
// Stage    : part2_stage12_coroutines
// Section  : section07_pitfalls
// Item     : by_reference_parameter_dangling
// Topic id : part2/stage12/section07/by_reference_parameter_dangling

#include "learn/example_support.hpp"

#include <coroutine>
#include <exception>
#include <string>
#include <utility>

namespace {

constexpr std::string_view topic_id = "part2/stage12/section07/by_reference_parameter_dangling";

class CharacterStream {
public:
    struct promise_type {
        char current{};
        std::exception_ptr exception;
        CharacterStream get_return_object() noexcept {
            return CharacterStream{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        std::suspend_always yield_value(char value) noexcept {
            current = value;
            return {};
        }
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit CharacterStream(Handle handle) noexcept : handle_(handle) {}
    ~CharacterStream() {
        if (handle_) {
            handle_.destroy();
        }
    }
    CharacterStream(const CharacterStream&) = delete;
    CharacterStream& operator=(const CharacterStream&) = delete;
    CharacterStream(CharacterStream&& other) noexcept : handle_(std::exchange(other.handle_, {})) {}
    CharacterStream& operator=(CharacterStream&&) = delete;

    bool next() {
        if (handle_.done()) {
            return false;
        }
        handle_.resume();
        if (handle_.promise().exception) {
            std::rethrow_exception(handle_.promise().exception);
        }
        return !handle_.done();
    }
    [[nodiscard]] char value() const noexcept { return handle_.promise().current; }

private:
    Handle handle_{};
};

CharacterStream characters_by_value(std::string text) {
    for (const char character : text) {
        co_yield character;
    }
}

CharacterStream characters_by_reference(const std::string& text) {
    for (const char character : text) {
        co_yield character;
    }
}

[[nodiscard]] std::string collect(CharacterStream stream) {
    std::string result;
    while (stream.next()) {
        result.push_back(stream.value());
    }
    return result;
}

#if 0
// Unsafe and intentionally inactive: the temporary dies before this lazy coroutine first resumes.
auto dangling = characters_by_reference(std::string{"temporary"});
auto undefined = collect(std::move(dangling));
#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{topic_id};

    LEARN_EXPECT_EQ(checks, collect(characters_by_value(std::string{"owned"})), "owned");

    const std::string owner = "borrowed";
    auto borrowed = characters_by_reference(owner);
    LEARN_EXPECT_EQ(checks, collect(std::move(borrowed)), "borrowed");
    // owner is declared before the stream, so it covers the stream's entire lifetime.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage12/section07/by_reference_parameter_dangling", run>;

}  // namespace
