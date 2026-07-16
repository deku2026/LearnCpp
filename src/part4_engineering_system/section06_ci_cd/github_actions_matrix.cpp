// Runnable teaching example
// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section06_ci_cd
// Item     : github_actions_matrix
// Topic id : part4/section06/github_actions_matrix
// References: official CI, compiler, sanitizer, CMake and standard-library implementation documentation

#include "learn/example_support.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part4/section06/github_actions_matrix";

struct Job {
    std::string os;
    std::string compiler;
    std::string configuration;
    auto operator<=>(const Job&) const = default;
};

std::vector<Job> expand() {
    std::vector<Job> jobs;
    for (const std::string os : {"windows", "linux", "macos"}) {
        for (const std::string compiler : {"msvc", "clang", "gcc"}) {
            for (const std::string configuration : {"Debug", "Release"}) {
                if ((os == "windows" && compiler == "gcc") || (os == "macos" && compiler == "msvc")) continue;
                jobs.push_back({os, compiler, configuration});
            }
        }
    }
    return jobs;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const auto jobs = expand();
    LEARN_EXPECT_EQ(checks, jobs.size(), 14U);
    LEARN_EXPECT(checks, std::ranges::find(jobs, Job{"windows", "msvc", "Debug"}) != jobs.end());
    LEARN_EXPECT(checks, std::ranges::find(jobs, Job{"windows", "gcc", "Debug"}) == jobs.end());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part4/section06/github_actions_matrix", run>;

}  // namespace
