// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : ccache_versus_sccache
// Topic id : part4/section01/ccache_versus_sccache
//
// Covers: compiler cache: ccache vs sccache

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // CMAKE_CXX_COMPILER_LAUNCHER=ccache
    std::string launcher = "ccache";
    LEARN_CHECK(launcher == "ccache" || launcher == "sccache");
}

void demo_intermediate() {
    // sccache: cloud-friendly; ccache: local classic
    LEARN_CHECK(true);
}

void demo_expert() {
    // Cache keys include flags, compiler, content hashes
    LEARN_CHECK(true);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/ccache_versus_sccache", run>;

}  // namespace
