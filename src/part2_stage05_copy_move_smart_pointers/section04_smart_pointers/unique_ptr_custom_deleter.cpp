// Runnable teaching example
// Doc      : 第2部分-阶段5-拷贝与移动语义-智能指针.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : unique_ptr_custom_deleter
// Topic id : part2/stage05/section04/unique_ptr_custom_deleter
// References: [unique.ptr.dltr]

#include "learn/example_support.hpp"

#include <memory>

namespace {

constexpr std::string_view kTopic = "part2/stage05/section04/unique_ptr_custom_deleter";

struct Resource {
    int id;
};

struct RecordingDeleter {
    int* releases;
    void operator()(Resource* resource) const noexcept {
        ++*releases;
        delete resource;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int releases = 0;
    {
        std::unique_ptr<Resource, RecordingDeleter> resource{new Resource{6}, RecordingDeleter{&releases}};
        LEARN_EXPECT_EQ(checks, resource->id, 6);
        LEARN_EXPECT_EQ(checks, releases, 0);
    }
    LEARN_EXPECT_EQ(checks, releases, 1);

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage05/section04/unique_ptr_custom_deleter", run>;

}  // namespace
