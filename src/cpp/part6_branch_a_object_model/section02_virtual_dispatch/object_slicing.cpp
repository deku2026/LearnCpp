// LearnCpp topic
// Doc      : part6-branch-a-object-model.md (A7 对象切片)
// Stage    : part6_branch_a_object_model
// Section  : section02_virtual_dispatch
// Item     : object_slicing
// Topic id : part6/a/section02/object_slicing
//
// 要点: 按值把派生对象拷到基类 → 丢失派生成员, vptr 变成基类 → 多态失效;
//       触发: 按值传参/返回、基类=派生、vector<Base> 存派生;
//       避免: 指针/引用/unique_ptr; 或禁止基类公开拷贝 / 抽象基类。
// 参考: C++ Core Guidelines C.67; [class.copy.ctor]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

struct Animal {
    int legs = 4;
    virtual ~Animal() = default;
    virtual std::string sound() const { return "(generic)"; }
    virtual std::string kind() const { return "Animal"; }
};

struct Dog : Animal {
    std::string breed = "Husky";
    std::string sound() const override { return "Woof!"; }
    std::string kind() const override { return "Dog"; }
};

struct Cat : Animal {
    bool indoor = true;
    std::string sound() const override { return "Meow"; }
    std::string kind() const override { return "Cat"; }
};

// 防御切片: 多态基类禁止公开拷贝 (C.67 风格)
struct AbstractShape {
    virtual ~AbstractShape() = default;
    virtual double area() const = 0;

    AbstractShape() = default;
    AbstractShape(const AbstractShape&) = delete;
    AbstractShape& operator=(const AbstractShape&) = delete;
    AbstractShape(AbstractShape&&) = delete;
    AbstractShape& operator=(AbstractShape&&) = delete;
};

struct Circle : AbstractShape {
    double r = 1.0;
    double area() const override { return 3.141592653589793 * r * r; }
};

void by_value(Animal a) {
    // 进入函数前已切片: a 是真正的 Animal
    assert(a.kind() == "Animal");
    assert(a.sound() == "(generic)");
}

void by_ref(const Animal& a) {
    // 保留动态类型
    assert(a.kind() == "Dog" || a.kind() == "Cat" || a.kind() == "Animal");
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== object slicing ===\n";

    Dog dog;
    dog.breed = "Akita";
    assert(dog.sound() == "Woof!");
    assert(dog.kind() == "Dog");

    // --- 触发 1: 按值初始化基类对象 ---
    Animal sliced = dog;
    assert(sliced.kind() == "Animal");
    assert(sliced.sound() == "(generic)");
    assert(sliced.legs == 4);
    // breed 已不存在于 sliced
    std::cout << "  sliced.sound() = " << sliced.sound() << " (lost Dog)\n";

    // --- 触发 2: 按值传参 ---
    by_value(dog);

    // --- 触发 3: 赋值 ---
    Animal dest;
    dest = dog;
    assert(dest.sound() == "(generic)");

    // --- 触发 4: 值语义容器 ---
    std::vector<Animal> zoo_bad;
    zoo_bad.push_back(Dog{});
    zoo_bad.push_back(Cat{});
    assert(zoo_bad[0].sound() == "(generic)");
    assert(zoo_bad[1].sound() == "(generic)");
    std::cout << "  vector<Animal> both generic (sliced)\n";

    // --- 正确: 引用 / 指针 / unique_ptr ---
    by_ref(dog);
    [[maybe_unused]] Animal* p = &dog;
    assert(p->sound() == "Woof!");

    std::vector<std::unique_ptr<Animal>> zoo_good;
    zoo_good.push_back(std::make_unique<Dog>());
    zoo_good.push_back(std::make_unique<Cat>());
    assert(zoo_good[0]->sound() == "Woof!");
    assert(zoo_good[1]->sound() == "Meow");
    std::cout << "  vector<unique_ptr<Animal>> preserves polymorphism\n";

    // --- 防御: 删除拷贝的抽象基类 ---
    Circle c;
    c.r = 2.0;
    [[maybe_unused]] AbstractShape* ps = &c;
    assert(ps->area() > 12.0 && ps->area() < 13.0);
    // AbstractShape copy = c;  // 编译错误: 已 delete
    // std::vector<AbstractShape> shapes; shapes.push_back(c);  // 亦不可

    // 成因小结:
    // Animal sliced = dog 调用 Animal 的拷贝构造, 只复制 Animal 成员;
    // sliced 的动态类型就是 Animal, vptr 指向 Animal vtable。

    std::cout << "object_slicing: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/a/section02/object_slicing", run>;

}  // namespace
