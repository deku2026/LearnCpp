// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section04_deducing_this_cpp23
// Item     : collapse_ref_qualified_overloads
// Topic id : part2/stage04/section04/collapse_ref_qualified_overloads
//
// Covers: one Self&& template replaces four ref-qualified getters

#include "learn/topic_registry.hpp"

#include <string>
#include <type_traits>
#include <utility>

namespace {

#if defined(__cpp_explicit_this_parameter)

class Widget {
    std::string data_;

public:
    explicit Widget(std::string d) : data_(std::move(d)) {}

    template <class Self>
    auto&& data(this Self&& self) {
        return std::forward<Self>(self).data_;
    }
};

void demo_basics() {
    Widget w{"hello"};
    auto& r = w.data();
    LEARN_CHECK(r == "hello");
    r = "world";
    LEARN_CHECK(w.data() == "world");
}

void demo_intermediate() {
    const Widget cw{"const"};
    const auto& r = cw.data();
    LEARN_CHECK(r == "const");
    static_assert(std::is_const_v<std::remove_reference_t<decltype(cw.data())>>);
}

void demo_expert() {
    std::string s = Widget{"tmp"}.data();
    LEARN_CHECK(s == "tmp");
}

#else

void demo_basics() {
    class Widget {
        std::string data_;

    public:
        explicit Widget(std::string d) : data_(std::move(d)) {}
        std::string& data() & { return data_; }
        const std::string& data() const& { return data_; }
        std::string data() && { return std::move(data_); }
    };
    Widget w{"hello"};
    LEARN_CHECK(w.data() == "hello");
    LEARN_CHECK(Widget{"tmp"}.data() == "tmp");
}

void demo_intermediate() {
    LEARN_CHECK(true);
}

void demo_expert() {
    LEARN_CHECK(true);
}

#endif

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section04/collapse_ref_qualified_overloads", run>;

}  // namespace
