// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : iwyu_intro
// Topic id : part4/section03/iwyu_intro
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section03/iwyu_intro";

std::set<std::string> missing_direct_headers(const std::set<std::string>& used_symbols,
                                             const std::set<std::string>& included,
                                             const std::map<std::string, std::string>& provider) {
    std::set<std::string> missing;
    for (const auto& symbol : used_symbols) {
        const auto& header = provider.at(symbol);
        if (!included.contains(header)) missing.insert(header);
    }
    return missing;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const std::map<std::string, std::string> provider{{"std::vector", "<vector>"}, {"std::string", "<string>"}};
    const auto missing = missing_direct_headers({"std::vector", "std::string"}, {"<vector>"}, provider);
    LEARN_EXPECT_EQ(checks, missing, std::set<std::string>({"<string>"}));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section03/iwyu_intro", run>;

}  // namespace
