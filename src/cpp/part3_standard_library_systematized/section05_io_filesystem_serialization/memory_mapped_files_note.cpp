// LearnCpp topic example
// Doc      : part3-standard-library-systematized.md
// Stage    : part3_standard_library_systematized
// Section  : section05_io_filesystem_serialization
// Item     : memory_mapped_files_note
// Topic id : part3/section05/memory_mapped_files_note
//
// Covers: memory-mapped files are platform API; std::filesystem for paths only

#include "learn/topic_registry.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

namespace {

void demo_basics() {
    // Standard C++ has no mmap; use OS APIs or libraries. Demo: load file into vector.
    const auto path = std::filesystem::temp_directory_path() / "learncpp_mmap_note.bin";
    {
        std::ofstream out(path, std::ios::binary);
        out.write("ABC", 3);
    }
    {
        std::ifstream in(path, std::ios::binary);
        std::vector<char> bytes(3);
        in.read(bytes.data(), 3);
        LEARN_CHECK(bytes[0] == 'A');
    }
    std::filesystem::remove(path);
}

void demo_intermediate() {
    // mmap benefits: random access without full copy; needs OS mapping
    LEARN_CHECK(sizeof(void*) >= 4);
}

void demo_expert() {
    // Prefer RAII wrappers around CreateFileMapping/mmap; unmap on destruction
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

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section05/memory_mapped_files_note", run>;

}  // namespace
