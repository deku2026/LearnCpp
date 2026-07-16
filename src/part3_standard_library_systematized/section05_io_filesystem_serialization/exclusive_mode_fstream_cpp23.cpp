// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : exclusive_mode_fstream_cpp23
// Topic id : part3/section05/exclusive_mode_fstream_cpp23
// References: C++23 [ios.openmode], P2467R1

#include "learn/example_support.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>
#include <string_view>
#include <system_error>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section05/exclusive_mode_fstream_cpp23";
namespace fs = std::filesystem;

class UniqueTempDirectory {
public:
    UniqueTempDirectory() {
        std::error_code error;
        const fs::path base = fs::temp_directory_path(error);
        if (error) {
            return;
        }
        const auto seed = static_cast<std::uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count());
        for (std::uint32_t attempt = 0; attempt < 64; ++attempt) {
            fs::path candidate = base / ("learncpp-noreplace-" + std::to_string(seed) + '-' + std::to_string(attempt));
            error.clear();
            if (fs::create_directory(candidate, error)) {
                path_ = std::move(candidate);
                return;
            }
        }
    }

    ~UniqueTempDirectory() {
        std::error_code ignored;
        if (!path_.empty()) {
            fs::remove_all(path_, ignored);
        }
    }

    [[nodiscard]] const fs::path& path() const noexcept { return path_; }

private:
    fs::path path_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_ios_noreplace) && __cpp_lib_ios_noreplace >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    UniqueTempDirectory temporary;
    LEARN_EXPECT(checks, !temporary.path().empty());
    if (temporary.path().empty()) {
        return checks.result();
    }

    const fs::path existing = temporary.path() / "existing.txt";
    {
        std::ofstream seed{existing};
        seed << "keep";
    }

    // noreplace requests an atomic exclusive create from the file-opening
    // operation; checking exists() before a normal open has a race window.
    std::ofstream rejected{existing, std::ios_base::out | std::ios_base::noreplace};
    LEARN_EXPECT(checks, !rejected.is_open());
    LEARN_EXPECT(checks, rejected.fail());

    const fs::path fresh = temporary.path() / "fresh.txt";
    {
        std::ofstream created{fresh, std::ios_base::out | std::ios_base::noreplace};
        LEARN_EXPECT(checks, created.is_open());
        created << "created once";
        LEARN_EXPECT(checks, created.good());
    }

    std::ifstream preserved{existing};
    std::ifstream new_file{fresh};
    std::string old_text;
    std::string new_text;
    std::getline(preserved, old_text);
    std::getline(new_file, new_text);
    LEARN_EXPECT_EQ(checks, old_text, std::string{"keep"});
    LEARN_EXPECT_EQ(checks, new_text, std::string{"created once"});
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::ios_base::noreplace");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/exclusive_mode_fstream_cpp23", run>;

}  // namespace
