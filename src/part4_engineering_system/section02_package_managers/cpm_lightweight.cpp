// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : cpm_lightweight
// Topic id : part4/section02/cpm_lightweight
// References: official CMake, Ninja, vcpkg, Conan, Clang and analyzer documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section02/cpm_lightweight";

struct FetchDeclaration {
    std::string_view repository;
    std::string_view revision;
    std::string_view integrity_hash;
};

bool pinned(const FetchDeclaration& declaration) {
    const bool immutable_revision = declaration.revision.size() == 40 || declaration.revision.starts_with('v');
    return !declaration.repository.empty() && immutable_revision && !declaration.integrity_hash.empty();
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const FetchDeclaration stable{"https://example.test/library.git", std::string_view{"v2.1.0"}, "sha256:abc"};
    const FetchDeclaration branch{"https://example.test/library.git", "main", {}};
    LEARN_EXPECT(checks, pinned(stable));
    LEARN_EXPECT(checks, !pinned(branch));
    // Lightweight fetching still needs version policy, offline/cache behavior and supply-chain verification.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section02/cpm_lightweight", run>;

}  // namespace
