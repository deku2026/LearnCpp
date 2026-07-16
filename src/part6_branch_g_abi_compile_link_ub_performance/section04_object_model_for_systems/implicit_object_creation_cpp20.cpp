// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : implicit_object_creation_cpp20
// Topic id : part6/g/section04/implicit_object_creation_cpp20

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>

namespace {

struct Packet {
    int id;
    int payload;
};

struct NonTrivial {
    explicit NonTrivial(int number) noexcept : value(number) {}
    ~NonTrivial() {}
    int value;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/implicit_object_creation_cpp20"};

    static_assert(std::is_trivially_copyable_v<Packet>);
    void* raw_packet = std::malloc(sizeof(Packet));
    if (raw_packet == nullptr) {
        return ::learn::ExampleChecks::unavailable("part6/g/section04/implicit_object_creation_cpp20",
                                                   "malloc storage");
    }
    auto* packet = static_cast<Packet*>(raw_packet);
    packet->id = 40;
    packet->payload = 2;
    LEARN_EXPECT_EQ(checks, packet->id + packet->payload, 42);
    std::free(raw_packet);

    void* raw_managed = ::operator new(sizeof(NonTrivial));
    auto* managed = std::construct_at(static_cast<NonTrivial*>(raw_managed), 42);
    LEARN_EXPECT_EQ(checks, managed->value, 42);
    std::destroy_at(managed);
    ::operator delete(raw_managed);

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/implicit_object_creation_cpp20", run>;

}  // namespace
