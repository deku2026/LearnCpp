// Topic     : [*this] 值捕获当前对象（C++17）
// Doc       : 第2部分-阶段3 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: [*this] 把 *this 拷贝进闭包；回调晚于对象销毁仍安全（基于副本）；
//       代价是拷贝；大对象可改为只捕获需要的成员。

#include "learn/topic_registry.hpp"

#include <functional>
#include <iostream>
#include <string>

namespace {

struct Widget {
    int id = 0;
    std::string name = "w";

    std::function<std::string()> label_by_this() const {
        return [this] { return name + "#" + std::to_string(id); };
    }

    std::function<std::string()> label_by_star_this() const {
        // C++17：拷贝整个 *this 进闭包
        return [*this] { return name + "#" + std::to_string(id); };
    }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [capture_star_this_cpp17] ===\n";

    // -------------------------------------------------------------------------
    // §入门：语法与立即使用
    // -------------------------------------------------------------------------
    Widget w{7, "alpha"};
    auto f = w.label_by_star_this();
    assert(f() == "alpha#7");
    std::cout << "[intro] [*this] copies object into closure\n";

    // -------------------------------------------------------------------------
    // §进阶：与 [this] 的寿命对比（安全路径）
    // -------------------------------------------------------------------------
    std::function<std::string()> delayed;
    {
        Widget temp{3, "temp"};
        delayed = temp.label_by_star_this();
        // temp 即将销毁；闭包内有副本
    }
    assert(delayed() == "temp#3");

    // [this] 版本在对象销毁后调用是 UB——此处不调用，只对比写法：
    // auto bad = Widget{1,"x"}.label_by_this(); // 临时 Widget 销毁后 bad 悬垂
    std::cout << "[advanced] [*this] survives object destruction; [this] would dangle\n";

    // -------------------------------------------------------------------------
    // §专家：拷贝代价、mutable 与 const
    // -------------------------------------------------------------------------
    Widget src{1, "src"};
    auto mut_copy = [obj = src]() mutable {
        obj.id = 99;  // 改副本
        return obj.id;
    };
    assert(mut_copy() == 99);
    assert(src.id == 1);

    // [*this] 在 const 成员函数中拷贝的是 const 视图下的对象；
    // 若要在 lambda 内修改副本，需要 mutable。
    // 大对象：优先 [id=id, name=name] 只抓字段。
    auto thin = [id = src.id, name = src.name] { return name + "#" + std::to_string(id); };
    assert(thin() == "src#1");

    std::cout << "[expert] prefer [*this] for async/store; or capture fields to cut cost\n";
    std::cout << "=== capture_star_this_cpp17: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_star_this_cpp17", run>;

}  // namespace
