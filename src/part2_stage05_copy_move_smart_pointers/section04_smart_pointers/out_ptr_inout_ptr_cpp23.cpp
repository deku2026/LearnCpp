// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : out_ptr_inout_ptr_cpp23
// Topic id : part2/stage05/section04/out_ptr_inout_ptr_cpp23
// References: [out.ptr], P1132R8, SD-FeatureTest __cpp_lib_out_ptr

#include "learn/example_support.hpp"

#include <memory>
#include <version>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/out_ptr_inout_ptr_cpp23";

void c_create(int** output) {
    *output = new int{42};
}

void c_replace(int** input_output) {
    int* const replacement = new int{7};
    delete *input_output;
    *input_output = replacement;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    std::unique_ptr<int> value;
    c_create(std::out_ptr(value));
    LEARN_EXPECT_EQ(checks, *value, 42);
    c_replace(std::inout_ptr(value));
    LEARN_EXPECT_EQ(checks, *value, 7);
#else
    int* raw = nullptr;
    c_create(&raw);
    std::unique_ptr<int> value{raw};
    LEARN_EXPECT_EQ(checks, *value, 42);
    raw = value.release();
    c_replace(&raw);
    value.reset(raw);
    LEARN_EXPECT_EQ(checks, *value, 7);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/out_ptr_inout_ptr_cpp23", run>;

}  // namespace
