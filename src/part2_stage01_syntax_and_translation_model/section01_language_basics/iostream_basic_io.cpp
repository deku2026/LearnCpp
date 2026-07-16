// Runnable teaching example
// Doc      : 第2部分-阶段1-语法基础与翻译模型.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section01_language_basics
// Item     : iostream_basic_io
// Topic id : part2/stage01/section01/iostream_basic_io
// Reference: N4950 [iostream.objects.overview], [ostream.manip], [istream.formatted].

#include "learn/example_support.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace {

class CountingBuffer : public std::stringbuf {
public:
    int sync() override {
        ++sync_count_;
        return std::stringbuf::sync();
    }

    [[nodiscard]] int sync_count() const noexcept { return sync_count_; }

private:
    int sync_count_{};
};

static_assert(std::is_same_v<decltype(std::cin), std::istream>);
static_assert(std::is_same_v<decltype(std::cout), std::ostream>);
static_assert(std::is_same_v<decltype(std::cerr), std::ostream>);
static_assert(std::is_same_v<decltype(std::clog), std::ostream>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    ::learn::ExampleChecks checks{"part2/stage01/section01/iostream_basic_io"};

    std::istringstream input{"42 Ada Lovelace\nnot-a-number"};
    int answer{};
    std::string name;
    input >> answer >> std::ws;
    std::getline(input, name);
    LEARN_EXPECT_EQ(checks, answer, 42);
    LEARN_EXPECT_EQ(checks, name, std::string{"Ada Lovelace"});

    int invalid_number{99};
    input >> invalid_number;
    LEARN_EXPECT(checks, input.fail());

    std::ostringstream formatted;
    formatted << std::boolalpha << true << ' ' << std::hex << 42;
    LEARN_EXPECT_EQ(checks, formatted.str(), std::string{"true 2a"});

    CountingBuffer buffer;
    std::ostream output{&buffer};
    output << "line" << '\n';
    LEARN_EXPECT_EQ(checks, buffer.sync_count(), 0);
    output << "flushed" << std::endl;
    LEARN_EXPECT_EQ(checks, buffer.sync_count(), 1);
    LEARN_EXPECT_EQ(checks, buffer.str(), std::string{"line\nflushed\n"});

    LEARN_EXPECT(checks, std::cin.rdbuf() != nullptr);
    LEARN_EXPECT(checks, std::cout.rdbuf() != nullptr);
    LEARN_EXPECT(checks, std::cerr.rdbuf() != nullptr);
    LEARN_EXPECT(checks, std::clog.rdbuf() != nullptr);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section01/iostream_basic_io", run>;

}  // namespace
