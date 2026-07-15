// LearnCpp topic example
// Doc      : part6-branch-g-abi-compile-link-ub-performance.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : vtable_abi_contract
// Topic id : part6/g/section01/vtable_abi_contract
//
// Covers: vtable layout is ABI contract; do not hardcode offsets

#include "learn/topic_registry.hpp"

#include <string>
#include <typeinfo>

namespace {

struct Interface {
    virtual int code() const { return 1; }
    virtual ~Interface() = default;
};

struct Impl : Interface {
    int code() const override { return 2; }
};

void demo_basics() {
    Impl x;
    Interface& i = x;
    LEARN_CHECK(i.code() == 2);
}

void demo_intermediate() {
    Impl x;
    Interface* p = &x;
    LEARN_CHECK(p->code() == 2);
}

void demo_expert() {
    LEARN_CHECK(std::string{typeid(Impl).name()}.size() > 0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/vtable_abi_contract", run>;

}  // namespace
