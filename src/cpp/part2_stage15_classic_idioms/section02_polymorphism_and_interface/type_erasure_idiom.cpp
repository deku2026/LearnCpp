// LearnCpp topic
// Doc      : part2-stage15-classic-idioms.md §2.3
// Stage    : part2_stage15_classic_idioms
// Section  : section02_polymorphism_and_interface
// Item     : type_erasure_idiom
// Topic id : part2/stage15/section02/type_erasure_idiom
//
// Type erasure: runtime value-semantic polymorphism without a shared base class.
// Motive : store unrelated types that satisfy a structural interface (like function).
// Modern : std::function, std::any, std::shared_ptr<void>+deleter, C++23
//          std::move_only_function; small-buffer optimization in production libs.
// Pitfall: heap + virtual dispatch cost; if closed set of types, prefer variant.

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <version>

#if !defined(__cpp_lib_move_only_function) || !(__cpp_lib_move_only_function)
namespace {
int run(int /*argc*/, char** /*argv*/) {
    std::cout << "[skip] std::move_only_function not available (__cpp_lib_move_only_function)\n";
    return 0;
}
[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/type_erasure_idiom", run>;
}  // namespace
#else
namespace {

// Teaching-grade type-erased "Drawable" (Sean Parent style, simplified).
class Drawable {
    struct Concept {
        virtual ~Concept() = default;
        virtual void draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template <typename T>
    struct Model final : Concept {
        T obj;
        explicit Model(T o) : obj(std::move(o)) {}
        void draw() const override { obj.draw(); }
        std::unique_ptr<Concept> clone() const override { return std::make_unique<Model>(*this); }
    };

    std::unique_ptr<Concept> self_;

public:
    template <typename T>
    Drawable(T obj) : self_(std::make_unique<Model<T>>(std::move(obj))) {}  // NOLINT

    Drawable(const Drawable& o) : self_(o.self_->clone()) {}
    Drawable& operator=(const Drawable& o) {
        Drawable tmp(o);
        swap(*this, tmp);
        return *this;
    }
    Drawable(Drawable&&) noexcept = default;
    Drawable& operator=(Drawable&&) noexcept = default;

    friend void swap(Drawable& a, Drawable& b) noexcept { a.self_.swap(b.self_); }

    void draw() const { self_->draw(); }
};

struct Circle {
    double r = 1;
    void draw() const { std::cout << "  circle r=" << r << '\n'; }
};

struct Square {
    double side = 1;
    void draw() const { std::cout << "  square s=" << side << '\n'; }
};

struct TextLabel {
    std::string text;
    void draw() const { std::cout << "  text \"" << text << "\"\n"; }
};

// No common base among Circle/Square/TextLabel — type erasure unifies them.

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "== hand-rolled type erasure (value semantic) ==\n";
    std::vector<Drawable> shapes;
    shapes.emplace_back(Circle{.r = 2.5});
    shapes.emplace_back(Square{.side = 3});
    shapes.emplace_back(TextLabel{.text = "hi"});

    for (const auto& s : shapes) {
        s.draw();
    }

    Drawable copy = shapes[0];  // deep clone via Concept::clone
    copy.draw();

    std::cout << "== library form: std::function ==\n";
    std::vector<std::function<void()>> jobs;
    jobs.emplace_back([] { std::cout << "  job A\n"; });
    int x = 0;
    jobs.emplace_back([&] {
        ++x;
        std::cout << "  job B x=" << x << '\n';
    });
    for (auto& j : jobs) {
        j();
    }
    assert(x == 1);

    std::cout << "== C++23 move_only_function (move-only callables) ==\n";
    auto up = std::make_unique<int>(42);
    std::move_only_function<int()> mo = [p = std::move(up)] { return *p; };
    assert(mo() == 42);
    std::cout << "  move_only_function holds unique_ptr-capturing lambda\n";

    std::cout << "when NOT to: closed type set → std::variant; hot path → measure heap cost\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage15/section02/type_erasure_idiom", run>;

}  // namespace
#endif  // __cpp_lib_move_only_function
