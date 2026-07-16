// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : spanstream_cpp23
// Topic id : part3/section05/spanstream_cpp23
// References: C++23 [spanstream], P0448R4

#include "learn/example_support.hpp"

#include <array>
#include <span>
#if __has_include(<spanstream>)
#include <spanstream>
#endif
#include <string_view>
#include <version>

namespace {

constexpr std::string_view kTopic = "part3/section05/spanstream_cpp23";

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_spanstream) && __cpp_lib_spanstream >= 202106L
    ::learn::ExampleChecks checks{kTopic};
    std::array<char, 64> storage{};
    std::ospanstream output{std::span<char>{storage}};
    output << "sensor " << 7 << ' ' << 12.5;
    LEARN_EXPECT(checks, output.good());

    const std::span<char> written = output.span();
    LEARN_EXPECT_EQ(checks, (std::string_view{written.data(), written.size()}), std::string_view{"sensor 7 12.5"});
    LEARN_EXPECT(checks, written.data() == storage.data());

    std::ispanstream input{written};
    std::string label;
    int id{};
    double reading{};
    input >> label >> id >> reading;
    LEARN_EXPECT_EQ(checks, label, std::string{"sensor"});
    LEARN_EXPECT_EQ(checks, id, 7);
    LEARN_EXPECT_EQ(checks, reading, 12.5);

    std::array<char, 4> tiny_storage{};
    std::ospanstream tiny{std::span<char>{tiny_storage}};
    tiny << "overflow";
    LEARN_EXPECT(checks, tiny.fail());
    LEARN_EXPECT(checks, tiny.span().size() <= tiny_storage.size());
    return checks.result();
#else
    return ::learn::ExampleChecks::unavailable(kTopic, "std::spanstream");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section05/spanstream_cpp23", run>;

}  // namespace
