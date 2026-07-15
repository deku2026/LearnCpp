// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : serialization_landscape_note
// Topic id : part3/section05/serialization_landscape_note
//
// Covers: serialization landscape: text/binary/schema libs vs raw bytes

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace {

void demo_basics() {
    // Minimal "serialization": fixed layout POD to bytes
    struct Packet {
        std::uint32_t id;
        std::uint32_t value;
    };
    Packet p{1, 42};
    std::vector<std::byte> bytes(sizeof(Packet));
    std::memcpy(bytes.data(), &p, sizeof(p));
    Packet q{};
    std::memcpy(&q, bytes.data(), sizeof(q));
    LEARN_CHECK(q.id == 1 && q.value == 42);
}

void demo_intermediate() {
    // Text forms (JSON/XML) need external libs; string building is not enough for production
    std::string json_like = "{\"n\":1}";
    LEARN_CHECK(json_like.find("n") != std::string::npos);
}

void demo_expert() {
    // Prefer schema evolution (protobuf/flatbuffers) for long-lived formats
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/serialization_landscape_note", run>;

}  // namespace
