// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : lsan_overview
// Topic id : part4/section05/lsan_overview

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <memory>
#include <string_view>
#include <vector>

namespace {

#if defined(__clang__)
#if __has_feature(leak_sanitizer)
constexpr bool leak_sanitizer_enabled = true;
#else
constexpr bool leak_sanitizer_enabled = false;
#endif
#elif defined(__SANITIZE_ADDRESS__)
constexpr bool leak_sanitizer_enabled = true;  // Common Unix ASan builds integrate LSan.
#else
constexpr bool leak_sanitizer_enabled = false;
#endif

struct Node {
    Node() { ++live_count; }
    ~Node() { --live_count; }

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    std::vector<std::shared_ptr<Node>> children;
    std::weak_ptr<Node> parent;  // A non-owning back edge prevents a reference cycle.
    inline static int live_count{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section05/lsan_overview"};

    {
        auto root = std::make_shared<Node>();
        auto child = std::make_shared<Node>();
        child->parent = root;
        root->children.push_back(child);
        LEARN_EXPECT_EQ(checks, Node::live_count, 2);
        LEARN_EXPECT(checks, !child->parent.expired());
    }
    LEARN_EXPECT_EQ(checks, Node::live_count, 0);

#if 0
    // Run as a separate expected-failure test with leak detection enabled.
    [[maybe_unused]] auto* leaked = new int[128];
#endif

    constexpr std::string_view standalone = "-fsanitize=leak";
    constexpr std::string_view integrated = "ASAN_OPTIONS=detect_leaks=1";
    constexpr std::string_view instrumentation =
        leak_sanitizer_enabled ? "LSan instrumentation is active" : "LSan instrumentation is not active";
    LEARN_EXPECT(checks, standalone.contains("leak"));
    LEARN_EXPECT(checks, integrated.contains("detect_leaks"));
    LEARN_EXPECT(checks, instrumentation.starts_with("LSan"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/lsan_overview", run>;

}  // namespace
