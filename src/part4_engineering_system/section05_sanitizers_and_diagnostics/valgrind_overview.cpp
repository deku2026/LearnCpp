// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : valgrind_overview
// Topic id : part4/section05/valgrind_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstddef>
#include <string_view>
#include <vector>

namespace {

class TrackedBuffer {
public:
    explicit TrackedBuffer(std::size_t size) : bytes_(size) { live_bytes += bytes_.size(); }
    ~TrackedBuffer() { live_bytes -= bytes_.size(); }

    TrackedBuffer(const TrackedBuffer&) = delete;
    TrackedBuffer& operator=(const TrackedBuffer&) = delete;
    TrackedBuffer(TrackedBuffer&&) = delete;
    TrackedBuffer& operator=(TrackedBuffer&&) = delete;

    [[nodiscard]] std::size_t size() const noexcept { return bytes_.size(); }
    inline static std::size_t live_bytes{};

private:
    std::vector<std::byte> bytes_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/valgrind_overview"};

    {
        TrackedBuffer request{128};
        TrackedBuffer response{256};
        LEARN_EXPECT_EQ(checks, request.size() + response.size(), 384U);
        LEARN_EXPECT_EQ(checks, TrackedBuffer::live_bytes, 384U);
    }
    LEARN_EXPECT_EQ(checks, TrackedBuffer::live_bytes, 0U);

    constexpr std::string_view command =
        "valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all "
        "--track-origins=yes ./unit_tests";
    LEARN_EXPECT(checks, command.contains("memcheck"));
    LEARN_EXPECT(checks, command.contains("track-origins"));

    constexpr std::string_view comparison =
        "Memcheck needs no compiler instrumentation but is slower; ASan is the usual fast CI lane";
    LEARN_EXPECT(checks, comparison.contains("no compiler instrumentation"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/valgrind_overview", run>;

}  // namespace
