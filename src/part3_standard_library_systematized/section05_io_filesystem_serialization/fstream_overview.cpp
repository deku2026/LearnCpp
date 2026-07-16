// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : fstream_overview
// Topic id : part3/section05/fstream_overview
// References: C++23 [fstreams], [filebuf], [iostreams.base]

#include "learn/example_support.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section05/fstream_overview";
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
            fs::path candidate = base / ("learncpp-fstream-" + std::to_string(seed) + '-' + std::to_string(attempt));
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

    UniqueTempDirectory(const UniqueTempDirectory&) = delete;
    UniqueTempDirectory& operator=(const UniqueTempDirectory&) = delete;

    [[nodiscard]] const fs::path& path() const noexcept { return path_; }

private:
    fs::path path_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    UniqueTempDirectory temporary;
    LEARN_EXPECT(checks, !temporary.path().empty());
    if (temporary.path().empty()) {
        return checks.result();
    }

    const fs::path file = temporary.path() / "records.txt";
    {
        std::ofstream output{file};
        LEARN_EXPECT(checks, output.is_open());
        output.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        output << "alpha 10\n" << "beta 20\n";
    }  // RAII closes and flushes the file.

    {
        std::ofstream append{file, std::ios_base::app};
        LEARN_EXPECT(checks, append.is_open());
        append << "gamma 30\n";
    }

    std::ifstream input{file};
    std::vector<std::string> names;
    int total{};
    std::string name;
    int amount{};
    while (input >> name >> amount) {
        names.push_back(name);
        total += amount;
    }
    LEARN_EXPECT(checks, input.eof());
    LEARN_EXPECT_EQ(checks, names.size(), std::size_t{3});
    LEARN_EXPECT_EQ(checks, names.front(), std::string{"alpha"});
    LEARN_EXPECT_EQ(checks, total, 60);
    input.close();

    // A bidirectional fstream needs an intervening seek (or other positioning
    // operation) when switching between writing and reading.
    {
        std::fstream update{file, std::ios_base::in | std::ios_base::out};
        LEARN_EXPECT(checks, update.is_open());
        update.seekp(0);
        update.write("ALPHA", 5);
        update.flush();
        update.seekg(0);
        std::string first;
        update >> first;
        LEARN_EXPECT_EQ(checks, first, std::string{"ALPHA"});
    }

    std::error_code error;
    LEARN_EXPECT(checks, fs::is_regular_file(file, error));
    LEARN_EXPECT(checks, !error);
    LEARN_EXPECT(checks, fs::file_size(file, error) > 0);
    LEARN_EXPECT(checks, !error);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/fstream_overview", run>;

}  // namespace
