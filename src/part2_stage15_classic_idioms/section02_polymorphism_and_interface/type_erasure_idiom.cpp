// Runnable teaching example
// Doc      : 第2部分-阶段15-C++经典惯用法.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : type_erasure_idiom
// Topic id : part2/stage15/section02/type_erasure_idiom
// References: C++23 [class.copy], [class.dtor], [temp], [expr.const], C++ Core Guidelines

#include "learn/example_support.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace {

constexpr std::string_view kTopic = "part2/stage15/section02/type_erasure_idiom";

class AnyLabel {
    struct Concept {
        virtual ~Concept() = default;
        virtual std::string label() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template <class T>
    struct Model final : Concept {
        explicit Model(T stored_value) : value(std::move(stored_value)) {}
        std::string label() const override { return value.label(); }
        std::unique_ptr<Concept> clone() const override { return std::make_unique<Model>(*this); }
        T value;
    };

public:
    template <class T>
    explicit AnyLabel(T value) : object_(std::make_unique<Model<T>>(std::move(value))) {}
    AnyLabel(const AnyLabel& other) : object_(other.object_->clone()) {}
    AnyLabel(AnyLabel&&) noexcept = default;
    AnyLabel& operator=(AnyLabel replacement) noexcept {
        object_.swap(replacement.object_);
        return *this;
    }
    std::string label() const { return object_->label(); }

private:
    std::unique_ptr<Concept> object_;
};

struct Number {
    int value;
    std::string label() const { return std::to_string(value); }
};
struct Word {
    std::string value;
    std::string label() const { return value; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{kTopic};
    AnyLabel first{Number{7}};
    AnyLabel second{Word{"ready"}};
    AnyLabel copy = second;
    LEARN_EXPECT_EQ(checks, first.label(), std::string{"7"});
    LEARN_EXPECT_EQ(checks, second.label(), std::string{"ready"});
    LEARN_EXPECT_EQ(checks, copy.label(), second.label());
    return checks.result();
}

[[maybe_unused]] const auto& registered = ::learn::topic<"part2/stage15/section02/type_erasure_idiom", run>;

}  // namespace
