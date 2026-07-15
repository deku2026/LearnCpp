// LearnCpp topic example
// Doc      : part2-stage15-classic-idioms.md
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : type_erasure_idiom
// Topic id : part2/stage15/section02/type_erasure_idiom
//
// Covers: type erasure small drawable

#include "learn/topic_registry.hpp"

#include <memory>
#include <utility>

namespace {

class Drawable {
    struct Concept {
        virtual ~Concept() = default;
        virtual int draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };
    template <class T>
    struct Model : Concept {
        T v;
        explicit Model(T x) : v(std::move(x)) {}
        int draw() const override { return v.draw(); }
        std::unique_ptr<Concept> clone() const override { return std::make_unique<Model>(*this); }
    };
    std::unique_ptr<Concept> self_;

public:
    template <class T>
    Drawable(T x) : self_(std::make_unique<Model<T>>(std::move(x))) {}
    Drawable(const Drawable& o) : self_(o.self_->clone()) {}
    Drawable& operator=(Drawable o) {
        self_.swap(o.self_);
        return *this;
    }
    int draw() const { return self_->draw(); }
};

struct Circle {
    int r;
    int draw() const { return r * r; }
};
struct Box {
    int w;
    int draw() const { return w; }
};

void demo_basics() {
    Drawable d = Circle{3};
    LEARN_CHECK(d.draw() == 9);
}

void demo_intermediate() {
    Drawable d = Box{4};
    Drawable c = d;
    LEARN_CHECK(c.draw() == 4);
}

void demo_expert() {
    Drawable d = Circle{2};
    d = Box{5};
    LEARN_CHECK(d.draw() == 5);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/type_erasure_idiom", run>;

}  // namespace
