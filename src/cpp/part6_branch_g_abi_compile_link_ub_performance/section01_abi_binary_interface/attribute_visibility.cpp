// LearnCpp topic
// Doc      : 第6部分-支线G · G2.2 visibility 属性
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section01_abi_binary_interface
// Item     : attribute_visibility
// Topic id : part6/g/section01/attribute_visibility
//
// 要点: GCC/Clang visibility("default"|"hidden"|"protected");
//       类上标注影响成员默认可见性。
// 参考: https://gcc.gnu.org/wiki/Visibility

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#define VIS_DEFAULT __attribute__((visibility("default")))
#define VIS_HIDDEN __attribute__((visibility("hidden")))
#else
#define VIS_DEFAULT
#define VIS_HIDDEN
#endif

namespace {

class VIS_DEFAULT ExportedWidget {
public:
    explicit ExportedWidget(int v) : v_(v) {}
    int get() const { return v_; }
    // 成员默认跟随类的 default

private:
    int v_;
};

class VIS_HIDDEN HiddenWidget {
public:
    int get() const { return 7; }
};

[[maybe_unused]] VIS_DEFAULT std::string exported_name() {
    return "public";
}
[[maybe_unused]] VIS_HIDDEN std::string hidden_name() {
    return "internal";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G2 attribute visibility ===\n";

    ExportedWidget w{42};
    assert(w.get() == 42);
    assert(HiddenWidget{}.get() == 7);
    assert(exported_name() == "public");
    assert(hidden_name() == "internal");

#if defined(__GNUC__) || defined(__clang__)
    std::cout << "  GCC/Clang attributes active\n";
#else
    std::cout << "  attributes no-op on this compiler; concept still applies\n";
#endif

    std::cout << "  pair with -fvisibility=hidden for shared libraries\n";
    std::cout << "attribute_visibility: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section01/attribute_visibility", run>;

}  // namespace
