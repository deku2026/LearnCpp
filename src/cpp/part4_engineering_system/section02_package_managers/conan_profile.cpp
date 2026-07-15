// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.2.2 Conan 2.x profile
// Stage    : part4_engineering_system
// Section  : section02_package_managers
// Item     : conan_profile
// Topic id : part4/section02/conan_profile
//
// 要点: profile 描述 os/arch/compiler/build_type; conan install 生成
//       CMakeToolchain + CMakeDeps。适合跨编译与精细二进制区分。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct Profile {
    std::string os;
    std::string arch;
    std::string compiler;
    std::string compiler_version;
    std::string build_type;
    std::string compiler_cppstd{"23"};
};

// 二进制包 id 关键(简化): 同依赖不同 profile → 不同包
std::string package_key(std::string_view dep, const Profile& p) {
    return std::string(dep) + "|" + p.os + "|" + p.arch + "|" + p.compiler + "|" + p.compiler_version + "|" +
           p.build_type + "|std" + p.compiler_cppstd;
}

bool profiles_equal(const Profile& a, const Profile& b) {
    return a.os == b.os && a.arch == b.arch && a.compiler == b.compiler && a.compiler_version == b.compiler_version &&
           a.build_type == b.build_type && a.compiler_cppstd == b.compiler_cppstd;
}

// 本机探测 → 建议 profile 字段
Profile detect_host_sketch() {
    Profile p;
#if defined(_WIN32)
    p.os = "Windows";
#elif defined(__APPLE__)
    p.os = "Macos";
#else
    p.os = "Linux";
#endif
#if defined(_M_X64) || defined(__x86_64__)
    p.arch = "x86_64";
#elif defined(_M_ARM64) || defined(__aarch64__)
    p.arch = "armv8";
#else
    p.arch = "unknown";
#endif
#if defined(__clang__)
    p.compiler = "clang";
    p.compiler_version = std::to_string(__clang_major__);
#elif defined(_MSC_VER)
    p.compiler = "msvc";
    p.compiler_version = std::to_string(_MSC_VER);
#elif defined(__GNUC__)
    p.compiler = "gcc";
    p.compiler_version = std::to_string(__GNUC__);
#else
    p.compiler = "unknown";
    p.compiler_version = "0";
#endif
#ifdef NDEBUG
    p.build_type = "Release";
#else
    p.build_type = "Debug";
#endif
    return p;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== conan_profile ===\n";

    // --- 入门: 文档 profile 字段 ---
    Profile linux_gcc{
        "Linux", "x86_64", "gcc", "13", "Release", "23",
    };
    Profile win_msvc{
        "Windows", "x86_64", "msvc", "194", "Debug", "23",
    };
    assert(!profiles_equal(linux_gcc, win_msvc));

    const auto k1 = package_key("fmt/10.2.1", linux_gcc);
    const auto k2 = package_key("fmt/10.2.1", win_msvc);
    assert(k1 != k2);
    std::cout << "  fmt package id differs across profiles\n";

    // --- 进阶: conanfile 依赖列表 ---
    std::vector<std::string> conan_requires{"fmt/10.2.1", "nlohmann_json/3.11.3"};
    std::vector<std::string> generators{"CMakeToolchain", "CMakeDeps"};
    assert(conan_requires.size() == 2);
    assert(generators[0] == "CMakeToolchain");

    // conan install . --output-folder=build --build=missing
    // cmake -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake
    const std::string conan_tc = "build/conan_toolchain.cmake";
    assert(conan_tc.find("conan_toolchain") != std::string::npos);

    // --- 专家: 本机 sketch ---
    Profile host = detect_host_sketch();
    std::cout << "  host sketch: " << host.os << "/" << host.arch << " " << host.compiler << "-"
              << host.compiler_version << " " << host.build_type << " cppstd=" << host.compiler_cppstd << '\n';
    assert(!host.os.empty());
    assert(host.compiler_cppstd == "23");

    // 跨编译: host profile ≠ build profile
    Profile cross = linux_gcc;
    cross.arch = "armv8";
    assert(package_key("zlib/1.3", linux_gcc) != package_key("zlib/1.3", cross));

    std::cout << "conan_profile: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section02/conan_profile", run>;

}  // namespace
