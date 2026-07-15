// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.1.2 target_link / include_directories
// Stage    : part4_engineering_system
// Section  : section01_build_system_cmake
// Item     : target_link_include_directories
// Topic id : part4/section01/target_link_include_directories
//
// 要点: target_include_directories + target_link_libraries 表达依赖;
//       BUILD_INTERFACE / INSTALL_INTERFACE 区分源码树与安装树路径。
// 仓库: target_include_directories(learn_cpp PRIVATE "${CMAKE_SOURCE_DIR}/include")

#include "learn/topic_registry.hpp"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Lib {
    std::string name;
    std::set<std::string> public_includes;
    std::set<std::string> private_includes;
};

struct Link {
    std::string consumer;
    std::string provider;
};

// 消费者编译 include = 自身 public+private + 直接链接方的 public includes
std::set<std::string> compile_includes_for(const Lib& self, const std::vector<Link>& links,
                                           const std::map<std::string, Lib>& all) {
    std::set<std::string> out = self.public_includes;
    out.insert(self.private_includes.begin(), self.private_includes.end());
    for (const auto& lk : links) {
        if (lk.consumer != self.name) {
            continue;
        }
        auto it = all.find(lk.provider);
        if (it == all.end()) {
            continue;
        }
        out.insert(it->second.public_includes.begin(), it->second.public_includes.end());
    }
    return out;
}

// 安装导出后路径: BUILD 用源树, INSTALL 用相对 include/
std::string select_interface_path(bool installing, std::string_view build_path, std::string_view install_path) {
    return std::string(installing ? install_path : build_path);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== target_link_include_directories ===\n";

    std::map<std::string, Lib> all;
    all["core"] = Lib{
        "core",
        {"${SRC}/include/core"},
        {"${SRC}/core/internal"},
    };
    all["app"] = Lib{"app", {}, {"${SRC}/app"}};

    std::vector<Link> links{{"app", "core"}};

    auto app_incs = compile_includes_for(all["app"], links, all);
    assert(app_incs.contains("${SRC}/include/core"));
    assert(app_incs.contains("${SRC}/app"));
    assert(!app_incs.contains("${SRC}/core/internal"));

    std::cout << "  app compile includes:";
    for (const auto& i : app_incs) {
        std::cout << " [" << i << "]";
    }
    std::cout << '\n';

    // BUILD_INTERFACE vs INSTALL_INTERFACE
    const auto build_p = select_interface_path(false, "${CMAKE_CURRENT_SOURCE_DIR}/include", "include");
    const auto install_p = select_interface_path(true, "${CMAKE_CURRENT_SOURCE_DIR}/include", "include");
    assert(build_p.find("CMAKE_CURRENT_SOURCE_DIR") != std::string::npos);
    assert(install_p == "include");
    std::cout << "  BUILD_INTERFACE -> " << build_p << '\n';
    std::cout << "  INSTALL_INTERFACE -> " << install_p << '\n';

    // 仓库 learn_cpp: 单可执行 PRIVATE include, 不向外导出
    Lib learn{"learn_cpp", {}, {"${CMAKE_SOURCE_DIR}/include"}};
    auto li = compile_includes_for(learn, {}, {{"learn_cpp", learn}});
    assert(li.size() == 1);
    assert(li.begin()->find("include") != std::string::npos);

    // target_link_libraries(app PRIVATE core) 后 app 能 #include 核心公共头
    assert(app_incs.contains("${SRC}/include/core"));

    std::cout << "target_link_include_directories: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section01/target_link_include_directories", run>;

}  // namespace
