// Runnable teaching example
// Doc      : 第2部分-阶段4-类与对象模型基础.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : copy_assignment
// Topic id : part2/stage04/section03/copy_assignment
// References: [class.copy.assign]

#include "learn/example_support.hpp"

#include <memory>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part2/stage04/section03/copy_assignment";

struct Samples {
    std::vector<int> values;
};

void copy_assign(Samples& destination, const Samples& source) {
    destination = source;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    Samples source{{1, 2, 3}};
    Samples target{{9}};
    target = source;

    LEARN_EXPECT_EQ(checks, target.values, source.values);
    LEARN_EXPECT(checks, target.values.data() != source.values.data());
    copy_assign(target, target);  // The generated member is safe when both references alias.
    LEARN_EXPECT_EQ(checks, target.values.size(), std::size_t{3});

    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage04/section03/copy_assignment", run>;

}  // namespace
