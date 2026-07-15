// LearnCpp topic example
// Doc      : part4-engineering-system.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : libfuzzer_intro
// Topic id : part4/section04/libfuzzer_intro
//
// Covers: libFuzzer harness conceptual (safe)

#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

void demo_basics() {
    // LLVMFuzzerTestOneInput(data, size)
    auto parse = [](const unsigned char* data, std::size_t n) { return n == 0 ? 0 : static_cast<int>(data[0]); };
    unsigned char buf[] = {1, 2};
    LEARN_CHECK(parse(buf, 2) == 1);
}

void demo_intermediate() {
    // Build with -fsanitize=fuzzer,address
    LEARN_CHECK(true);
}

void demo_expert() {
    // Corpus + coverage guided mutation
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/libfuzzer_intro", run>;

}  // namespace
