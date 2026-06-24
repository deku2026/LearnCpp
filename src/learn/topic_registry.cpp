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

TopicRegistrar::TopicRegistrar(std::string_view id, TopicFn fn) noexcept {
    registry().emplace(std::string{id}, fn);
}

int run_topic(int argc, char** argv) {
    if (argc < 1 || argv[0] == nullptr) {
        std::cerr << "learn_cpp: missing topic id\n";
        return 2;
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
