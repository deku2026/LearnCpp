// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : flat_containers_cpp23
// Topic id : part3/section02/flat_containers_cpp23
// Reference: WG21 P0429R9 (std::flat_map) and P1222R4 (std::flat_set).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<flat_map>)
#include <flat_map>
#define LEARN_HAS_FLAT_MAP_HEADER 1
#else
#define LEARN_HAS_FLAT_MAP_HEADER 0
#endif
#if __has_include(<flat_set>)
#include <flat_set>
#define LEARN_HAS_FLAT_SET_HEADER 1
#else
#define LEARN_HAS_FLAT_SET_HEADER 0
#endif

#include <algorithm>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/flat_containers_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_FLAT_MAP_HEADER && LEARN_HAS_FLAT_SET_HEADER && defined(__cpp_lib_flat_map) && \
    __cpp_lib_flat_map >= 202207L && defined(__cpp_lib_flat_set) && __cpp_lib_flat_set >= 202207L
    std::flat_map<std::string, int, std::less<>> index{{"beta", 2}, {"alpha", 1}};
    index.try_emplace("gamma", 3);
    LEARN_EXPECT_EQ(checks, index.begin()->first, std::string{"alpha"});
    LEARN_EXPECT_EQ(checks, index.at("gamma"), 3);
    LEARN_EXPECT(checks, index.contains("beta"));

    std::flat_set<int> unique{4, 1, 3, 1};
    std::flat_multiset<int> repeated{2, 1, 2};
    std::flat_multimap<std::string, int> aliases{{"x", 1}, {"x", 2}};
    LEARN_EXPECT_EQ(checks, unique.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, *unique.begin(), 1);
    LEARN_EXPECT_EQ(checks, repeated.count(2), std::size_t{2});
    LEARN_EXPECT_EQ(checks, aliases.count("x"), std::size_t{2});

    // Flat containers maintain sorted contiguous backing sequences: lookup is logarithmic and
    // traversal is cache-friendly, while middle insertion is linear and may invalidate iterators.
    return checks.result();
#else
    // A vector kept sorted models the central trade-off without pretending to be a full substitute.
    std::vector<std::pair<std::string, int>> modeled{{"beta", 2}, {"alpha", 1}};
    std::ranges::sort(modeled, {}, &std::pair<std::string, int>::first);
    const auto found = std::ranges::lower_bound(modeled, std::string{"beta"}, {}, &std::pair<std::string, int>::first);
    LEARN_EXPECT(checks, found != modeled.end());
    LEARN_EXPECT_EQ(checks, found->second, 2);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_flat_map and __cpp_lib_flat_set >= 202207L");
#endif
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/flat_containers_cpp23", run>;

}  // namespace
