// A variadic forwarding wrapper preserves every constructor argument's value category.
// Doc      : 第2部分-阶段6-模板与泛型编程.md
// Stage    : part2_stage06_templates_and_generics
// Section  : section02_variadic_templates
// Item     : variadic_perfect_forwarding
// Topic id : part2/stage06/section02/variadic_perfect_forwarding
//
// The lvalue name is copied while the unique_ptr rvalue transfers ownership exactly once.

#include "learn/example_support.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace {

struct Job {
    std::string name;
    std::unique_ptr<int> code;

    Job(std::string job_name, std::unique_ptr<int> job_code) : name(std::move(job_name)), code(std::move(job_code)) {}
};

template <class T, class... Arguments>
std::unique_ptr<T> make_owned(Arguments&&... arguments) {
    return std::unique_ptr<T>{new T(std::forward<Arguments>(arguments)...)};
}

static_assert(std::is_constructible_v<Job, std::string&, std::unique_ptr<int>&&>);

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    learn::ExampleChecks checks{"part2/stage06/section02/variadic_perfect_forwarding"};
    std::string name{"compile"};
    auto code = std::make_unique<int>(23);
    auto job = make_owned<Job>(name, std::move(code));

    LEARN_EXPECT_EQ(checks, name, std::string{"compile"});
    LEARN_EXPECT(checks, code == nullptr);
    LEARN_EXPECT_EQ(checks, job->name, std::string{"compile"});
    LEARN_EXPECT_EQ(checks, *job->code, 23);
    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage06/section02/variadic_perfect_forwarding", run>;

}  // namespace
