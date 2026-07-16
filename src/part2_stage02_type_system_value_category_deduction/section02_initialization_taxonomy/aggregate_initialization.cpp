// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section02_initialization_taxonomy
// Item     : aggregate_initialization
// Topic id : part2/stage02/section02/aggregate_initialization
//

#include "learn/example_support.hpp"

#include <string_view>
#include <type_traits>

namespace {

struct Address {
    int number;
    std::string_view street;
};

struct Contact {
    std::string_view name;
    Address address;
    bool active{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    static_assert(std::is_aggregate_v<Address>);
    static_assert(std::is_aggregate_v<Contact>);

    Contact full{"Ada", {12, "Compiler Lane"}, true};
    Contact partial{"Bjarne", {7, "Standard Way"}};  // active is value-initialized.

    learn::ExampleChecks checks{"part2/stage02/section02/aggregate_initialization"};
    LEARN_EXPECT_EQ(checks, full.address.number, 12);
    LEARN_EXPECT(checks, full.active);
    LEARN_EXPECT(checks, !partial.active);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section02/aggregate_initialization", run>;

}  // namespace
