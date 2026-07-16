// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section02_containers_and_algorithms
// Item     : heterogeneous_erase_extract_cpp23
// Topic id : part3/section02/heterogeneous_erase_extract_cpp23
// Reference: WG21 P2077R3 (__cpp_lib_associative_heterogeneous_erasure).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif

#include <map>
#include <string>
#include <string_view>
#include <utility>

namespace {

using Index = std::map<std::string, int, std::less<>>;

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section02/heterogeneous_erase_extract_cpp23";
    learn::ExampleChecks checks{topic};
    Index index{{"alpha", 1}, {"beta", 2}, {"gamma", 3}};

    // std::less<> is transparent, so lookup does not allocate a temporary std::string.
    const std::string_view alpha{"alpha"};
    LEARN_EXPECT(checks, index.find(alpha) != index.end());

#if defined(__cpp_lib_associative_heterogeneous_erasure) && __cpp_lib_associative_heterogeneous_erasure >= 202110L
    LEARN_EXPECT_EQ(checks, index.erase(alpha), std::size_t{1});
    auto node = index.extract(std::string_view{"beta"});
#else
    // C++17-compatible fallback preserves heterogeneous lookup, then erases/extracts by iterator.
    index.erase(index.find(alpha));
    auto node = index.extract(index.find(std::string_view{"beta"}));
#endif
    LEARN_EXPECT(checks, !index.contains("alpha"));
    LEARN_EXPECT(checks, !node.empty());

    // A node handle owns the extracted element. Its key can be changed safely before reinsertion.
    node.key() = "delta";
    node.mapped() = 4;
    const auto inserted = index.insert(std::move(node));
    LEARN_EXPECT(checks, inserted.inserted);
    LEARN_EXPECT(checks, inserted.node.empty());
    LEARN_EXPECT_EQ(checks, index.at("delta"), 4);
    LEARN_EXPECT_EQ(checks, index.size(), std::size_t{2});

    // Heterogeneous erase participates only when the comparator/hash and equality are transparent;
    // iterator overloads are excluded from the forwarding overload to avoid ambiguity.
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section02/heterogeneous_erase_extract_cpp23", run>;

}  // namespace
