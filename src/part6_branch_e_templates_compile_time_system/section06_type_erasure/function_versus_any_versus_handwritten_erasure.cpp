// Doc      : Cpp-Modern-完整学习路线图-C++23.md
// Doc anchor: 支线 E · 模板与编译期系统(深水 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section06_type_erasure
// Item     : function_versus_any_versus_handwritten_erasure
// Topic id : part6/e/section06/function_versus_any_versus_handwritten_erasure

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace {

class Renderable {
private:
    struct Interface {
        virtual ~Interface() = default;
        [[nodiscard]] virtual std::string render() const = 0;
    };

    template <class T>
    struct Model final : Interface {
        explicit Model(T value) : value_(std::move(value)) {}
        [[nodiscard]] std::string render() const override { return value_.render(); }
        T value_;
    };

public:
    template <class T>
        requires requires(const T& value) {
            { value.render() } -> std::same_as<std::string>;
        }
    explicit Renderable(T value) : object_(std::make_shared<Model<T>>(std::move(value))) {}

    [[nodiscard]] std::string render() const { return object_->render(); }

private:
    std::shared_ptr<const Interface> object_;
};

struct Label {
    std::string text;
    [[nodiscard]] std::string render() const { return "[" + text + "]"; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part6/e/section06/function_versus_any_versus_handwritten_erasure"};

    std::function<int(int)> callable = [offset = 2](int value) { return value + offset; };
    LEARN_EXPECT_EQ(checks, callable(40), 42);

    std::any payload = std::string{"runtime value with explicit cast"};
    LEARN_EXPECT_EQ(checks, std::any_cast<const std::string&>(payload).size(), 32U);
    LEARN_EXPECT(checks, std::any_cast<int>(&payload) == nullptr);

    const Renderable erased{Label{"open set"}};
    const Renderable copied = erased;  // Shared, immutable value-semantics model.
    LEARN_EXPECT_EQ(checks, erased.render(), std::string{"[open set]"});
    LEARN_EXPECT_EQ(checks, copied.render(), erased.render());

    return checks.result();
}

[[maybe_unused]] const auto& _ =
    ::learn::topic<"part6/e/section06/function_versus_any_versus_handwritten_erasure", run>;

}  // namespace
