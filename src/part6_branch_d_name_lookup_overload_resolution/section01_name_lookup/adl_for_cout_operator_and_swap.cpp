// Runnable teaching example
// Doc      : 第6部分-支线D-名字查找ADL与重载决议.md
// Stage    : part6_branch_d_name_lookup_overload_resolution
// Section  : section01_name_lookup
// Item     : adl_for_cout_operator_and_swap
// Topic id : part6/d/section01/adl_for_cout_operator_and_swap
// References: C++23 [basic.lookup], [temp.res], [over.match]; P2387R3, P1895R0

#include "learn/example_support.hpp"

#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part6/d/section01/adl_for_cout_operator_and_swap";

namespace library {

struct Widget {
    int id{};
    std::string name;
    int* swaps{};

    friend std::ostream& operator<<(std::ostream& output, const Widget& value) {
        return output << "Widget(" << value.id << "," << value.name << ")";
    }

    friend void swap(Widget& left, Widget& right) noexcept {
        using std::swap;
        swap(left.id, right.id);
        swap(left.name, right.name);
        swap(left.swaps, right.swaps);
        if (left.swaps != nullptr) {
            ++*left.swaps;
        }
    }
};

}  // namespace library

template <class T>
void adl_swap(T& left, T& right) noexcept(noexcept(swap(left, right))) {
    using std::swap;
    swap(left, right);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};

    int swap_count{};
    library::Widget left{1, "left", &swap_count};
    library::Widget right{2, "right", &swap_count};

    std::ostringstream output;
    output << left;  // operator<< is found in library by ADL.
    LEARN_EXPECT_EQ(checks, output.str(), std::string{"Widget(1,left)"});

    adl_swap(left, right);
    LEARN_EXPECT_EQ(checks, left.id, 2);
    LEARN_EXPECT_EQ(checks, right.name, std::string{"left"});
    LEARN_EXPECT_EQ(checks, swap_count, 1);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/d/section01/adl_for_cout_operator_and_swap", run>;

}  // namespace
