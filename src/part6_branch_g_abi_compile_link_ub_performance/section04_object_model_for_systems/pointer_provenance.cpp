// Doc      : 第6部分-支线G-ABI编译链接UB性能.md
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section04_object_model_for_systems
// Item     : pointer_provenance
// Topic id : part6/g/section04/pointer_provenance

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <span>

namespace {

template <class T>
struct DerivedPointer {
    std::span<T> allocation;
    std::size_t offset;

    [[nodiscard]] std::optional<T*> resolve() const noexcept {
        if (offset >= allocation.size()) {
            return std::nullopt;
        }
        return allocation.data() + offset;
    }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/g/section04/pointer_provenance"};

    std::array values{10, 20, 30};
    DerivedPointer<int> middle{values, 1};
    LEARN_EXPECT(checks, middle.resolve().has_value());
    LEARN_EXPECT_EQ(checks, **middle.resolve(), 20);

    DerivedPointer<int> one_past{values, values.size()};
    LEARN_EXPECT(checks, !one_past.resolve().has_value());
    int* begin = values.data();
    int* end = begin + values.size();
    LEARN_EXPECT_EQ(checks, end - begin, static_cast<std::ptrdiff_t>(values.size()));

#if 0
    *end = 7;  // It retains values' provenance but is not dereferenceable.
    std::array<int, 1> first{};
    std::array<int, 1> second{};
    int* first_end = first.data() + first.size();
    if (first_end == second.data()) {
        *first_end = 1;  // Numeric equality cannot change its one-past provenance.
    }
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section04/pointer_provenance", run>;

}  // namespace
