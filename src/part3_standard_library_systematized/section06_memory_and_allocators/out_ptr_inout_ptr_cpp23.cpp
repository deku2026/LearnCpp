// Runnable teaching example
// Doc      : 第3部分-标准库系统化.md
// Stage    : part3_standard_library_systematized
// Section  : section06_memory_and_allocators
// Item     : out_ptr_inout_ptr_cpp23
// Topic id : part3/section06/out_ptr_inout_ptr_cpp23
// References: C++23 [out.ptr.t], [inout.ptr.t], P1132R8

#include "learn/example_support.hpp"

#include <memory>
#include <string_view>

namespace {

constexpr std::string_view kTopic = "part3/section06/out_ptr_inout_ptr_cpp23";

void c_create_integer(int** output, int value) {
    if (output != nullptr) {
        *output = new int{value};
    }
}

void c_replace_integer(int** input_output, int value) {
    if (input_output != nullptr) {
        int* const replacement = new int{value};
        delete *input_output;
        *input_output = replacement;
    }
}

bool c_try_create_integer(int** output, bool succeed) {
    if (!succeed) {
        return false;  // A failed C API must document whether it touches *output.
    }
    c_create_integer(output, 99);
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
#if defined(__cpp_lib_out_ptr) && __cpp_lib_out_ptr >= 202106L
    ::learn::ExampleChecks checks{kTopic};

    std::unique_ptr<int> handle;
    c_create_integer(std::out_ptr(handle), 42);
    LEARN_EXPECT(checks, handle != nullptr);
    LEARN_EXPECT_EQ(checks, *handle, 42);

    // inout_ptr releases the old raw pointer to a C API that consumes/replaces
    // it, then puts the returned pointer back into the smart pointer.
    c_replace_integer(std::inout_ptr(handle), 84);
    LEARN_EXPECT_EQ(checks, *handle, 84);

    // out_ptr resets existing ownership before accepting a pure output value.
    const bool created = c_try_create_integer(std::out_ptr(handle), true);
    LEARN_EXPECT(checks, created);
    LEARN_EXPECT_EQ(checks, *handle, 99);

    // On this API's documented failure path *output is untouched. out_ptr's
    // temporary starts as null, so the smart pointer becomes empty safely.
    const bool rejected = c_try_create_integer(std::out_ptr(handle), false);
    LEARN_EXPECT(checks, !rejected);
    LEARN_EXPECT(checks, handle == nullptr);
    return checks.result();
#else
    // Manual ownership hand-off models what the C++23 adapters automate.
    ::learn::ExampleChecks checks{kTopic};
    std::unique_ptr<int> handle;
    int* raw = nullptr;

    c_create_integer(&raw, 42);
    handle.reset(raw);
    LEARN_EXPECT_EQ(checks, *handle, 42);

    raw = handle.release();
    try {
        c_replace_integer(&raw, 84);
    } catch (...) {
        // c_replace_integer allocates before consuming the old pointer. If
        // allocation throws, raw still denotes that object, so restore the
        // unique owner before propagating the exception.
        handle.reset(raw);
        throw;
    }
    handle.reset(raw);
    LEARN_EXPECT_EQ(checks, *handle, 84);

    handle.reset();
    raw = nullptr;
    const bool created = c_try_create_integer(&raw, true);
    handle.reset(raw);
    LEARN_EXPECT(checks, created);
    LEARN_EXPECT_EQ(checks, *handle, 99);

    // out_ptr starts its temporary output slot at null. Model that reset
    // explicitly before the documented no-write failure path.
    handle.reset();
    raw = nullptr;
    const bool rejected = c_try_create_integer(&raw, false);
    handle.reset(raw);
    LEARN_EXPECT(checks, !rejected);
    LEARN_EXPECT(checks, handle == nullptr);
    if (const int result = checks.result(); result != 0) {
        return result;
    }
    return ::learn::ExampleChecks::unavailable(kTopic, "std::out_ptr/std::inout_ptr");
#endif
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part3/section06/out_ptr_inout_ptr_cpp23", run>;

}  // namespace
