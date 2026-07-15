// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §1.1
// Stage    : part2_stage15_classic_idioms
// Section  : section01_resource_lifetime
// Item     : raii_idiom
// Topic id : part2/stage15/section01/raii_idiom
//
// RAII: Resource Acquisition Is Initialization.
// Motive : bind resource lifetime to object lifetime so exception paths cannot leak.
// Modern : unique_ptr / lock_guard / fstream are library RAII; hand-roll only for
//          domain handles that the STL does not cover.
// Pitfall: never put non-owning "cleanup" in a destructor without a clear owner;
//          double free and use-after-free usually come from mixing raw + RAII.

#include "learn/topic_registry.hpp"

#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

// Minimal teaching RAII wrapper around a C FILE* (like a tiny unique_ptr for files).
class FileHandle {
    std::FILE* fp_ = nullptr;

public:
    FileHandle() = default;

    explicit FileHandle(const char* path, const char* mode) {
#if defined(_MSC_VER)
        // MSVC CRT marks fopen deprecated; fopen_s is the supported twin.
        if (fopen_s(&fp_, path, mode) != 0) {
            fp_ = nullptr;
        }
#else
        fp_ = std::fopen(path, mode);
#endif
        if (!fp_) {
            throw std::runtime_error(std::string{"fopen failed: "} + path);
        }
    }

    ~FileHandle() {
        if (fp_) {
            std::fclose(fp_);
            fp_ = nullptr;
            std::cout << "  [FileHandle dtor] closed\n";
        }
    }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& o) noexcept : fp_(std::exchange(o.fp_, nullptr)) {}
    FileHandle& operator=(FileHandle&& o) noexcept {
        if (this != &o) {
            if (fp_) {
                std::fclose(fp_);
            }
            fp_ = std::exchange(o.fp_, nullptr);
        }
        return *this;
    }

    std::FILE* get() const noexcept { return fp_; }

    void write(std::string_view s) {
        assert(fp_);
        std::fwrite(s.data(), 1, s.size(), fp_);
    }
};

void demo_manual_leak_risk() {
    std::cout << "== without RAII (manual, fragile) ==\n";
    int* p = new int{42};
    // if (true) throw ...;  // would leak
    delete p;
    std::cout << "  manual new/delete only safe on the happy path\n";
}

void demo_raii_stack_unwinding() {
    std::cout << "== RAII + stack unwinding ==\n";
    try {
        auto p = std::make_unique<int>(7);
        std::mutex m;
        std::lock_guard lock(m);  // unlocks even if we throw
        std::cout << "  unique_ptr owns " << *p << ", lock_guard holds mutex\n";
        throw std::runtime_error("boom");
    } catch (const std::exception& e) {
        std::cout << "  caught: " << e.what() << " (unique_ptr + lock already cleaned up)\n";
    }
}

void demo_custom_file_raii() {
    std::cout << "== custom FileHandle RAII ==\n";
    {
        FileHandle f("raii_demo_tmp.txt", "wb");
        f.write("hello RAII\n");
        std::cout << "  wrote temp file; leaving scope...\n";
    }  // dtor closes automatically
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    demo_manual_leak_risk();
    demo_raii_stack_unwinding();
    demo_custom_file_raii();

    std::cout << "when NOT to hand-roll: prefer unique_ptr/shared_ptr/lock_guard/fstream\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section01/raii_idiom", run>;

}  // namespace
