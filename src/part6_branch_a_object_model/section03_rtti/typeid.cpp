// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section03_rtti
// Item     : typeid
// Topic id : part6/a/section03/typeid
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <string_view>
#include <typeinfo>

namespace {

constexpr std::string_view kTopic = "part6/a/section03/typeid";

struct StaticBase {};
struct StaticDerived : StaticBase {};
struct PolyBase {
    virtual ~PolyBase() = default;
};
struct PolyDerived : PolyBase {};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    StaticDerived static_object;
    StaticBase& static_ref = static_object;
    LEARN_EXPECT(checks, typeid(static_ref) == typeid(StaticBase));
    PolyDerived poly_object;
    PolyBase& poly_ref = poly_object;
    LEARN_EXPECT(checks, typeid(poly_ref) == typeid(PolyDerived));
    LEARN_EXPECT(checks, typeid(PolyBase) != typeid(PolyDerived));
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section03/typeid", run>;

}  // namespace
