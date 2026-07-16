// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : rule_of_three
// Topic id : part2/stage04/section03/rule_of_three
// References: [class.copy.ctor], [class.copy.assign], [class.dtor]

#include "learn/example_support.hpp"

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/rule_of_three";

class LegacyBuffer {
public:
    explicit LegacyBuffer(std::size_t size) : size_(size), data_(new int[size]{}) {}
    ~LegacyBuffer() { delete[] data_; }

    LegacyBuffer(const LegacyBuffer& other) : LegacyBuffer(other.size_) {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    LegacyBuffer& operator=(const LegacyBuffer& other) {
        if (this != &other) {
            LegacyBuffer copy{other};
            std::swap(size_, copy.size_);
            std::swap(data_, copy.data_);
        }
        return *this;
    }

    int& operator[](std::size_t index) noexcept { return data_[index]; }
    [[nodiscard]] const int* data() const noexcept { return data_; }

private:
    std::size_t size_{};
    int* data_{};
};

// is_move_constructible can be true even without a move constructor: const T& can bind an rvalue.
static_assert(std::is_move_constructible_v<LegacyBuffer>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    LegacyBuffer source{1};
    source[0] = 11;
    const int* source_address = source.data();
    LegacyBuffer copied_from_rvalue{std::move(source)};

    LEARN_EXPECT_EQ(checks, source[0], 11);
    LEARN_EXPECT_EQ(checks, copied_from_rvalue[0], 11);
    LEARN_EXPECT_EQ(checks, source.data(), source_address);
    LEARN_EXPECT(checks, copied_from_rvalue.data() != source.data());

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/rule_of_three", run>;

}  // namespace
