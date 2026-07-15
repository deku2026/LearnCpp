// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section03_special_member_functions
// Item     : rule_of_zero
// Topic id : part2/stage04/section03/rule_of_zero
//
// Covers: Rule of 0 — compose RAII members, write no special members

#include "learn/topic_registry.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

class Person {
    std::string name_;
    std::vector<int> scores_;
    std::unique_ptr<int> data_;

public:
    explicit Person(std::string name) : name_(std::move(name)), data_(std::make_unique<int>(0)) {}

    const std::string& name() const { return name_; }
    void add_score(int s) { scores_.push_back(s); }
    int score_count() const { return static_cast<int>(scores_.size()); }
    void set_data(int v) { *data_ = v; }
    int data() const { return *data_; }
};

void demo_basics() {
    Person p{"Ada"};
    p.add_score(10);
    p.set_data(7);
    LEARN_CHECK(p.name() == "Ada");
    LEARN_CHECK(p.score_count() == 1);
    LEARN_CHECK(p.data() == 7);
}

void demo_intermediate() {
    Person a{"A"};
    a.add_score(1);
    Person b = std::move(a);
    LEARN_CHECK(b.name() == "A");
    LEARN_CHECK(b.score_count() == 1);
    // unique_ptr member => Person is not copyable
    static_assert(!std::is_copy_constructible_v<Person>);
    static_assert(std::is_move_constructible_v<Person>);
}

void demo_expert() {
    // Rule of 0: compiler-generated special members are correct because members
    // already own their resources.
    Person p{"Z"};
    p.set_data(3);
    Person q = std::move(p);
    LEARN_CHECK(q.data() == 3);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/rule_of_zero", run>;

}  // namespace
