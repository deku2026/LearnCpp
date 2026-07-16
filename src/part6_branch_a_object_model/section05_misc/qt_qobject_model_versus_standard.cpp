// Runnable teaching example
// Doc      : 第6部分-支线A-对象模型.md
// Stage    : part6_branch_a_object_model
// Section  : section05_misc
// Item     : qt_qobject_model_versus_standard
// Topic id : part6/a/section05/qt_qobject_model_versus_standard
// References: C++23 [intro.object], [basic.types], [class.mem], [class.virtual], [expr.dynamic.cast], [expr.typeid]

#include "learn/example_support.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::string_view kTopic = "part6/a/section05/qt_qobject_model_versus_standard";

class Observable {
public:
    using Callback = std::function<void(int)>;
    void connect(Callback callback) { callbacks_.push_back(std::move(callback)); }
    void publish(int value) const {
        for (const auto& callback : callbacks_) callback(value);
    }

private:
    std::vector<Callback> callbacks_;
};

struct Comparison {
    std::string_view mechanism;
    bool needs_external_codegen;
    bool has_runtime_metadata;
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    Observable observable;
    int received{};
    observable.connect([&](int value) { received = value; });
    observable.publish(23);
    LEARN_EXPECT_EQ(checks, received, 23);
    const Comparison standard{"virtuals/templates/callbacks", false, false};
    const Comparison qt_model{"meta-object signals/properties", true, true};
    LEARN_EXPECT(checks, !standard.needs_external_codegen && qt_model.needs_external_codegen);
    LEARN_EXPECT(checks, qt_model.has_runtime_metadata);
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part6/a/section05/qt_qobject_model_versus_standard", run>;

}  // namespace
