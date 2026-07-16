// Doc      : 第2部分-阶段10-STL深潜.md
// Stage    : part2_stage10_stl_deep_dive
// Section  : section07_ranges_core_cpp20
// Item     : projection_argument
// Topic id : part2/stage10/section07/projection_argument
//

#include "learn/example_support.hpp"

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <string>
#include <vector>

namespace {

struct Person {
    std::string name;
    int age;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    learn::ExampleChecks checks{"part2/stage10/section07/projection_argument"};
    std::vector<Person> people{{"Ada", 36}, {"Bob", 28}, {"Grace", 31}};

    std::ranges::sort(people, std::ranges::less{}, &Person::age);
    const auto bob = std::ranges::find(people, std::string{"Bob"}, &Person::name);
    const auto adults_over_thirty =
        std::ranges::count(people, true, [](const Person& person) { return person.age > 30; });

    std::vector<std::string> sorted_names;
    for (const Person& person : people) {
        sorted_names.push_back(person.name);
    }

    LEARN_EXPECT_EQ(checks, sorted_names, (std::vector<std::string>{"Bob", "Grace", "Ada"}));
    LEARN_EXPECT(checks, bob != people.end());
    LEARN_EXPECT_EQ(checks, bob->age, 28);
    LEARN_EXPECT_EQ(checks, adults_over_thirty, std::ptrdiff_t{2});

#if 0
    // The second argument is the comparator; a projection belongs in the third slot.
    std::ranges::sort(people, &Person::age);
#endif

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section07/projection_argument", run>;

}  // namespace
