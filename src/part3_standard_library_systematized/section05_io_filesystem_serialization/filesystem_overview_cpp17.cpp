// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : filesystem_overview_cpp17
// Topic id : part3/section05/filesystem_overview_cpp17
// References: C++23 [filesystems], [fs.op.funcs], [fs.class.path]

#include "learn/example_support.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section05/filesystem_overview_cpp17";
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
            fs::path candidate = base / ("learncpp-filesystem-" + std::to_string(seed) + '-' + std::to_string(attempt));
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

void write_text(const fs::path& path, std::string_view text) {
    std::ofstream output{path, std::ios_base::binary};
    output.write(text.data(), static_cast<std::streamsize>(text.size()));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    UniqueTempDirectory temporary;
    LEARN_EXPECT(checks, !temporary.path().empty());
    if (temporary.path().empty()) {
        return checks.result();
    }

    std::error_code error;
    const fs::path reports = temporary.path() / "data" / "reports";
    LEARN_EXPECT(checks, fs::create_directories(reports, error));
    LEARN_EXPECT(checks, !error);
    write_text(reports / "alpha.txt", "alpha");
    write_text(reports / "beta.log", "beta-data");

    const fs::path alpha = reports / "alpha.txt";
    LEARN_EXPECT(checks, fs::exists(alpha, error));
    LEARN_EXPECT(checks, !error);
    LEARN_EXPECT(checks, fs::is_regular_file(alpha, error));
    LEARN_EXPECT_EQ(checks, fs::file_size(alpha, error), std::uintmax_t{5});
    LEARN_EXPECT_EQ(checks, alpha.filename(), fs::path{"alpha.txt"});
    LEARN_EXPECT_EQ(checks, alpha.stem(), fs::path{"alpha"});
    LEARN_EXPECT_EQ(checks, alpha.extension(), fs::path{".txt"});

    std::vector<fs::path> names;
    for (fs::directory_iterator iterator{reports, error}, end; !error && iterator != end; iterator.increment(error)) {
        if (iterator->is_regular_file(error) && !error) {
            names.push_back(iterator->path().filename());
        }
    }
    std::ranges::sort(names);
    LEARN_EXPECT_EQ(checks, names, (std::vector<fs::path>{"alpha.txt", "beta.log"}));

    const fs::path relative = fs::relative(alpha, temporary.path(), error);
    LEARN_EXPECT(checks, !error);
    LEARN_EXPECT_EQ(checks, relative.generic_string(), std::string{"data/reports/alpha.txt"});
    const fs::path normalized = (reports / ".." / "reports" / "." / "alpha.txt").lexically_normal();
    LEARN_EXPECT_EQ(checks, fs::weakly_canonical(normalized, error), fs::weakly_canonical(alpha, error));

    const fs::path copied = reports / "copy.txt";
    LEARN_EXPECT(checks, fs::copy_file(alpha, copied, fs::copy_options::none, error));
    LEARN_EXPECT(checks, !error);
    const fs::path renamed = reports / "renamed.txt";
    fs::rename(copied, renamed, error);
    LEARN_EXPECT(checks, !error);
    LEARN_EXPECT(checks, fs::exists(renamed, error));

    // Error-code overloads make expected failures non-throwing.
    error.clear();
    const auto missing_size = fs::file_size(reports / "missing.txt", error);
    LEARN_EXPECT(checks, static_cast<bool>(error));
    LEARN_EXPECT_EQ(checks, missing_size, static_cast<std::uintmax_t>(-1));
    error.clear();
    LEARN_EXPECT(checks, fs::remove(renamed, error));
    LEARN_EXPECT(checks, !error);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/filesystem_overview_cpp17", run>;

}  // namespace
