// Runnable teaching example
// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section11_other_stl
// Item     : exclusive_mode_fstream_cpp23
// Topic id : part2/stage10/section11/exclusive_mode_fstream_cpp23
// References: C++23 [customization.point.object], [format], [print], [time], [filesystems], [coro.generator]

#include "learn/example_support.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage10/section11/exclusive_mode_fstream_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_ios_noreplace) && __cpp_lib_ios_noreplace >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / ("learncpp-noreplace-" + std::to_string(nonce) + ".tmp");
    std::ofstream first{path, std::ios::out | std::ios::noreplace};
    LEARN_EXPECT(checks, first.is_open());
    first << "owned";
    first.close();
    std::ofstream second{path, std::ios::out | std::ios::noreplace};
    LEARN_EXPECT(checks, !second.is_open());
    std::error_code error;
    const bool removed = std::filesystem::remove(path, error);
    LEARN_EXPECT(checks, removed && !error);
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::ios::noreplace");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage10/section11/exclusive_mode_fstream_cpp23", run>;

}  // namespace
