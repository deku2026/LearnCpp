// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : rule_of_five
// Topic id : part2/stage04/section03/rule_of_five
// References: [class.copy.ctor], [class.copy.assign], [class.dtor]

#include "learn/example_support.hpp"

#include <algorithm>
#include <cstddef>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/rule_of_five";

class IntBuffer {
public:
    explicit IntBuffer(std::size_t size) : size_(size), data_(new int[size]{}) {}
    ~IntBuffer() { delete[] data_; }

    IntBuffer(const IntBuffer& other) : IntBuffer(other.size_) {
        std::copy(other.data_, other.data_ + other.size_, data_);
    }

    IntBuffer& operator=(const IntBuffer& other) {
        IntBuffer copy{other};
        swap(copy);
        return *this;
    }

    IntBuffer(IntBuffer&& other) noexcept
        : size_(std::exchange(other.size_, 0)), data_(std::exchange(other.data_, nullptr)) {}

    IntBuffer& operator=(IntBuffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = std::exchange(other.size_, 0);
            data_ = std::exchange(other.data_, nullptr);
        }
        return *this;
    }

    void swap(IntBuffer& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    int& operator[](std::size_t index) noexcept { return data_[index]; }
    const int& operator[](std::size_t index) const noexcept { return data_[index]; }

private:
    std::size_t size_{};
    int* data_{};
};

void copy_assign(IntBuffer& destination, const IntBuffer& source) {
    destination = source;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    IntBuffer original{2};
    original[0] = 7;
    IntBuffer copied{original};
    copied[0] = 9;
    LEARN_EXPECT_EQ(checks, original[0], 7);
    LEARN_EXPECT_EQ(checks, copied[0], 9);

    IntBuffer moved{std::move(copied)};
    LEARN_EXPECT_EQ(checks, copied.size(), std::size_t{0});
    LEARN_EXPECT_EQ(checks, moved.size(), std::size_t{2});
    LEARN_EXPECT_EQ(checks, moved[0], 9);

    copy_assign(original, original);  // copy-and-swap handles aliased source/destination.
    LEARN_EXPECT_EQ(checks, original[0], 7);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/rule_of_five", run>;

}  // namespace
