// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : multiple_inheritance_multiple_vptr
// Topic id : part6/a/section02/multiple_inheritance_multiple_vptr
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>

namespace {

constexpr std::string_view kTopic = "part6/a/section02/multiple_inheritance_multiple_vptr";

struct Readable {
    virtual ~Readable() = default;
    virtual int read() const = 0;
};
struct Writable {
    virtual ~Writable() = default;
    virtual void write(int) = 0;
};

struct Device final : Readable, Writable {
    int read() const override { return value; }
    void write(int input) override { value = input; }
    int value{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Device device;
    Writable* writer = &device;
    Readable* reader = &device;
    writer->write(23);
    LEARN_EXPECT_EQ(checks, reader->read(), 23);
    LEARN_EXPECT(checks, dynamic_cast<Device*>(writer) == &device);
    LEARN_EXPECT(checks, dynamic_cast<Readable*>(writer) == reader);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section02/multiple_inheritance_multiple_vptr", run>;

}  // namespace
