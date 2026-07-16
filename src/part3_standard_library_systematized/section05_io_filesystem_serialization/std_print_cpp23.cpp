// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : std_print_cpp23
// Topic id : part3/section05/std_print_cpp23
// References: C++23 [print.fun], P2093R14

#include "learn/example_support.hpp"

#include <array>
#include <cstdio>
#include <memory>
#if __has_include(<print>)
#include <print>
#endif
#include <string>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section05/std_print_cpp23";

struct FileCloser {
    void operator()(std::FILE* file) const noexcept {
        if (file != nullptr) {
            static_cast<void>(std::fclose(file));
        }
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    ::learn::ExampleChecks checks{kTopic};
    using FileHandle = std::unique_ptr<std::FILE, FileCloser>;
    std::FILE* raw_file{};
#if defined(_MSC_VER)
    static_cast<void>(::tmpfile_s(&raw_file));
#else
    raw_file = std::tmpfile();
#endif
    FileHandle file{raw_file};
    LEARN_EXPECT(checks, file != nullptr);
    if (!file) {
        return checks.result();
    }

    // The format string is checked against the argument types at compile time.
    std::print(file.get(), "item={} price={:.2f}\n", "book", 12.5);
    std::println(file.get(), "count={:04}", 3);
    LEARN_EXPECT_EQ(checks, std::fflush(file.get()), 0);
    std::rewind(file.get());

    std::array<char, 128> buffer{};
    const std::size_t count = std::fread(buffer.data(), 1, buffer.size(), file.get());
    LEARN_EXPECT(checks, std::ferror(file.get()) == 0);
    const std::string output{buffer.data(), count};
    LEARN_EXPECT_EQ(checks, output, std::string{"item=book price=12.50\ncount=0003\n"});

#if 0
    // Intentionally ill-formed negative example: {:d} cannot format text.
    std::print("{:d}", "text");
#endif
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::print/std::println");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/std_print_cpp23", run>;

}  // namespace
