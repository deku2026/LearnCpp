// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : memory_mapped_files_note
// Topic id : part3/section05/memory_mapped_files_note
// References: C++23 [fstreams], [span]; memory mapping itself is platform-specific

#include "learn/example_support.hpp"

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part3/section05/memory_mapped_files_note";
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
            fs::path candidate = base / ("learncpp-file-image-" + std::to_string(seed) + '-' + std::to_string(attempt));
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

std::optional<std::vector<std::byte>> read_file_image(const fs::path& path) {
    std::error_code error;
    const std::uintmax_t file_size = fs::file_size(path, error);
    if (error || file_size > static_cast<std::uintmax_t>(std::numeric_limits<std::size_t>::max()) ||
        file_size > static_cast<std::uintmax_t>(std::numeric_limits<std::streamsize>::max())) {
        return std::nullopt;
    }

    std::vector<std::byte> bytes(static_cast<std::size_t>(file_size));
    std::ifstream input{path, std::ios_base::binary};
    if (!input) {
        return std::nullopt;
    }
    if (!bytes.empty()) {
        input.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        if (input.gcount() != static_cast<std::streamsize>(bytes.size())) {
            return std::nullopt;
        }
    }
    return bytes;
}

std::optional<std::span<const std::byte>> checked_window(std::span<const std::byte> image, std::size_t offset,
                                                         std::size_t length) {
    if (offset > image.size() || length > image.size() - offset) {
        return std::nullopt;
    }
    return image.subspan(offset, length);
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

    const fs::path file = temporary.path() / "image.bin";
    constexpr std::array<unsigned char, 8> payload{0x4C, 0x43, 0x50, 0x50, 10, 20, 30, 40};
    {
        std::ofstream output{file, std::ios_base::binary};
        output.write(reinterpret_cast<const char*>(payload.data()), static_cast<std::streamsize>(payload.size()));
    }

    // The C++23 standard library has no memory-mapping facility. Reading an
    // owned byte image is a portable fallback; OS mmap handles require their
    // own RAII wrapper and a lifetime longer than every exposed span.
    auto image = read_file_image(file);
    LEARN_EXPECT(checks, image.has_value());
    if (!image) {
        return checks.result();
    }
    LEARN_EXPECT_EQ(checks, image->size(), payload.size());

    const auto magic = checked_window(*image, 0, 4);
    LEARN_EXPECT(checks, magic.has_value());
    if (magic) {
        LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>((*magic)[0]), 0x4CU);
        LEARN_EXPECT_EQ(checks, std::to_integer<unsigned int>((*magic)[3]), 0x50U);
    }

    const auto data = checked_window(*image, 4, 4);
    LEARN_EXPECT(checks, data.has_value());
    unsigned int checksum{};
    if (data) {
        for (const std::byte byte : *data) {
            checksum += std::to_integer<unsigned int>(byte);
        }
    }
    LEARN_EXPECT_EQ(checks, checksum, 100U);
    LEARN_EXPECT(checks, !checked_window(*image, 7, 2));
    LEARN_EXPECT(checks, !checked_window(*image, std::numeric_limits<std::size_t>::max(), 1));
    LEARN_EXPECT(checks, !read_file_image(temporary.path() / "missing.bin"));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/memory_mapped_files_note", run>;

}  // namespace
