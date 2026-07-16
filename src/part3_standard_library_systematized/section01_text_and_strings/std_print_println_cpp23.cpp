// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_print_println_cpp23
// Topic id : part3/section01/std_print_println_cpp23
// Reference: WG21 P2093R14 (__cpp_lib_print).

#include "learn/example_support.hpp"

#if __has_include(<version>)
#include <version>
#endif
#if __has_include(<print>)
#include <print>
#define LEARN_HAS_PRINT_HEADER 1
#else
#define LEARN_HAS_PRINT_HEADER 0
#endif

#include <array>
#include <cstdio>
#include <string>
#include <string_view>

namespace {

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    constexpr auto topic = "part3/section01/std_print_println_cpp23";
    learn::ExampleChecks checks{topic};

#if LEARN_HAS_PRINT_HEADER && defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
    std::FILE* stream{};
#if defined(_WIN32)
    const auto open_error = ::tmpfile_s(&stream);
    LEARN_EXPECT_EQ(checks, open_error, 0);
#else
    stream = std::tmpfile();
#endif
    LEARN_EXPECT(checks, stream != nullptr);
    if (stream == nullptr) {
        return checks.result();
    }

    std::print(stream, "{} + {} = {}", 1, 2, 3);
    std::println(stream, "; {}", "done");
    LEARN_EXPECT_EQ(checks, std::fflush(stream), 0);
    std::rewind(stream);

    std::array<char, 64> buffer{};
    const auto count = std::fread(buffer.data(), sizeof(char), buffer.size(), stream);
    LEARN_EXPECT_EQ(checks, (std::string_view{buffer.data(), count}), std::string_view{"1 + 2 = 3; done\n"});
    LEARN_EXPECT_EQ(checks, std::fclose(stream), 0);
    // print emits no implicit newline; println adds exactly one. Literal formats retain compile-time checking.
#else
    std::array<char, 32> buffer{};
    const int count = std::snprintf(buffer.data(), buffer.size(), "%d + %d = %d", 1, 2, 3);
    LEARN_EXPECT(checks, count >= 0);
    LEARN_EXPECT(checks, count < static_cast<int>(buffer.size()));
    if (count >= 0 && static_cast<std::size_t>(count) < buffer.size()) {
        LEARN_EXPECT_EQ(checks, (std::string{buffer.data(), static_cast<std::size_t>(count)}),
                        std::string{"1 + 2 = 3"});
    }
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return learn::ExampleChecks::unavailable(topic, "__cpp_lib_print >= 202207L and <print>");
#endif
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_print_println_cpp23", run>;

}  // namespace
