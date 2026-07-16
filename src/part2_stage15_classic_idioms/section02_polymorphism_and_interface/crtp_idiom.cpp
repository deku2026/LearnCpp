// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : crtp_idiom
// Topic id : part2/stage15/section02/crtp_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <string>
#include <string_view>
#include <type_traits>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section02/crtp_idiom";

template <class Derived>
class Named {
public:
    std::string describe() const {
        const auto& self = static_cast<const Derived&>(*this);
        return "name=" + self.name_impl();
    }
};

class User final : public Named<User> {
public:
    explicit User(std::string name) : name_(std::move(name)) {}
    const std::string& name_impl() const noexcept { return name_; }

private:
    std::string name_;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    const User user{"Ada"};
    LEARN_EXPECT_EQ(checks, user.describe(), std::string{"name=Ada"});
    static_assert(!std::is_polymorphic_v<User>);  // Static polymorphism needs no virtual dispatch.
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section02/crtp_idiom", run>;

}  // namespace
