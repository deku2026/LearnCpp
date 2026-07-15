// LearnCpp topic example
// Doc      : part2-stage05-copy-move-smart-pointers.md
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section04_smart_pointers
// Item     : make_unique
// Topic id : part2/stage05/section04/make_unique
//
// Covers: prefer make_unique over unique_ptr(new T); exception safety

#include "learn/topic_registry.hpp"

#include <memory>
#include <string>
#include <utility>

namespace {

struct Widget {
    int id;
    std::string name;
    Widget(int i, std::string n) : id(i), name(std::move(n)) {}
};

template <class T, class... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void demo_basics() {
    auto p = std::make_unique<int>(42);
    LEARN_CHECK(p != nullptr);
    LEARN_CHECK(*p == 42);
}

void demo_intermediate() {
    auto w = std::make_unique<Widget>(7, "seven");
    LEARN_CHECK(w->id == 7);
    LEARN_CHECK(w->name == "seven");

    auto arr = std::make_unique<int[]>(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    LEARN_CHECK(arr[2] == 3);
}

void demo_expert() {
    auto q = my_make_unique<Widget>(1, std::string{"custom"});
    LEARN_CHECK(q->id == 1);
    LEARN_CHECK(q->name == "custom");

    // make_unique avoids naked new at call sites.
    auto release_and_rebuild = [](std::unique_ptr<int> p) {
        p.reset();
        p = std::make_unique<int>(99);
        return p;
    };
    auto r = release_and_rebuild(std::make_unique<int>(1));
    LEARN_CHECK(*r == 99);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section04/make_unique", run>;

}  // namespace
