#include "learn/topic_registry.hpp"

#include <iostream>
#include <map>
#include <string>

namespace learn {

namespace {

// Construct-on-first-use; safe under static-init across translation units.
std::map<std::string, TopicFn>& registry() noexcept {
    static std::map<std::string, TopicFn> instance;
    return instance;
}

}  // namespace

namespace detail {

void register_topic_impl(std::string_view id, TopicFn fn) noexcept {
    registry().emplace(std::string{id}, fn);
}

}  // namespace detail

int run_topic(int argc, char** argv) {
    if (argc < 1 || argv[0] == nullptr) {
#ifndef NDEBUG
        // Debug build with no topic id: iterate the whole map and call each.
        // Hit F5 in the IDE and any breakpoint you've set in any topic fires.
        auto& r = registry();
        std::cout << "learn_cpp [debug]: iterating " << r.size() << " topics\n";
        int failures = 0;
        for (const auto& [id, fn] : r) {
            const int rc = fn(argc, argv);
            if (rc != 0) {
                std::cerr << "  ! " << id << " returned " << rc << '\n';
                ++failures;
            }
        }
        if (failures > 0) {
            std::cerr << "learn_cpp [debug]: " << failures << " topic(s) returned non-zero\n";
        }
        return failures == 0 ? 0 : 1;
#else
        std::cerr << "learn_cpp: missing topic id\n";
        return 2;
#endif
    }
    const std::string id{argv[0]};
    auto& r = registry();
    const auto it = r.find(id);
    if (it == r.end()) {
        std::cerr << "learn_cpp: unknown topic '" << id << "'\n"
                  << "  run with no args to list available topics\n";
        return 2;
    }
    return it->second(argc, argv);
}

void list_topics() {
    auto& r = registry();
    std::cout << "learn_cpp: " << r.size() << " topics registered\n";
    for (const auto& [id, _] : r) {
        std::cout << "  " << id << '\n';
    }
}

}  // namespace learn
