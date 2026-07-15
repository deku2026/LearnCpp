// LearnCpp topic example
// Doc      : part2-stage04-classes-and-object-model-basics.md
// Stage    : part2_stage04_classes_and_object_model_basics
// Section  : section01_class_basics
// Item     : members_and_access_control
// Topic id : part2/stage04/section01/members_and_access_control
//
// Covers: public/private/protected, const member functions, class vs struct defaults

#include "learn/topic_registry.hpp"

#include <cassert>

namespace {

class Account {
public:
    explicit Account(double initial) : balance_(initial) {}

    void deposit(double amt) { balance_ += amt; }
    double balance() const { return balance_; }

protected:
    double raw_balance() const { return balance_; }

private:
    double balance_;
};

struct Point {
    int x = 0;
    int y = 0;
};

class Derived : public Account {
public:
    using Account::Account;
    double peek() const { return raw_balance(); }
};

void demo_basics() {
    Account a{100.0};
    a.deposit(25.0);
    assert(a.balance() == 125.0);

    const Account c{10.0};
    assert(c.balance() == 10.0);
}

void demo_intermediate() {
    Point p{3, 4};
    assert(p.x == 3 && p.y == 4);

    Derived d{50.0};
    assert(d.balance() == 50.0);
    assert(d.peek() == 50.0);
}

void demo_expert() {
    // class default access is private; struct default is public.
    // const member functions are callable on const objects.
    Account a{1.0};
    const Account& r = a;
    assert(r.balance() == 1.0);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section01/members_and_access_control", run>;

}  // namespace
