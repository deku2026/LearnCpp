// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : catch2_intro
// Topic id : part4/section04/catch2_intro

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <string_view>
#include <vector>

namespace {

class Cart {
public:
    void add(int price) { prices_.push_back(price); }
    [[nodiscard]] int total() const noexcept {
        int result = 0;
        for (const int price : prices_) {
            result += price;
        }
        return result;
    }
    [[nodiscard]] std::size_t size() const noexcept { return prices_.size(); }

private:
    std::vector<int> prices_;
};

template <class Section>
[[nodiscard]] bool run_independent_section(Section section) {
    Cart freshly_arranged_cart;  // Catch2 re-enters the test case for each SECTION path.
    return section(freshly_arranged_cart);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/catch2_intro"};

    const bool empty_section =
        run_independent_section([](const Cart& cart) { return cart.size() == 0 && cart.total() == 0; });
    const bool add_section = run_independent_section([](Cart& cart) {
        cart.add(20);
        cart.add(22);
        return cart.size() == 2 && cart.total() == 42;
    });
    LEARN_EXPECT(checks, empty_section);
    LEARN_EXPECT(checks, add_section);

    // GENERATE-style table: one property, several independently reported values.
    constexpr std::array prices{0, 1, 7, 42};
    for (const int price : prices) {
        LEARN_EXPECT(checks, run_independent_section([price](Cart& cart) {
                         cart.add(price);
                         return cart.total() == price;
                     }));
    }

    constexpr std::string_view cmake =
        "target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain); "
        "catch_discover_tests(unit_tests)";
    LEARN_EXPECT(checks, cmake.contains("Catch2WithMain"));
    LEARN_EXPECT(checks, cmake.contains("catch_discover_tests"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/catch2_intro", run>;

}  // namespace
