// LearnCpp topic example
// Doc      : part2-stage01-syntax-and-translation-model.md
// Stage    : part2_stage01_syntax_and_translation_model
// Section  : section03_translation_model
// Item     : per_cpp_object_file_independence
// Topic id : part2/stage01/section03/per_cpp_object_file_independence
//
// Covers: each .cpp compiles independently to its own .o with private static state

#include "learn/topic_registry.hpp"

#include <string>
#include <utility>

namespace {

// Real build: a.cpp → a.o, b.cpp → b.o. Compiling a.cpp does not need b.cpp's
// source; only the linker needs both object files when symbols are referenced.
//
// We simulate two logical TUs with separate namespaces, each owning static state.

namespace tu_alpha {
// Would be internal to a.cpp (anonymous ns / static).
namespace {
int hits = 0;
std::string label = "alpha";
}  // namespace

void touch() {
    ++hits;
}

int hit_count() {
    return hits;
}

const std::string& name() {
    return label;
}

int exported_sum(int x) {
    // "Exported" symbol from alpha.o — callable after link.
    return x + hits;
}
}  // namespace tu_alpha

namespace tu_beta {
namespace {
int hits = 0;
std::string label = "beta";
}  // namespace

void touch() {
    ++hits;
}

int hit_count() {
    return hits;
}

const std::string& name() {
    return label;
}

int exported_sum(int x) {
    return x + hits;
}
}  // namespace tu_beta

void demo_basics() {
    // Independent object files start with independent static storage.
    LEARN_CHECK(tu_alpha::hit_count() == 0);
    LEARN_CHECK(tu_beta::hit_count() == 0);
    LEARN_CHECK(tu_alpha::name() == "alpha");
    LEARN_CHECK(tu_beta::name() == "beta");
}

void demo_intermediate() {
    // Mutations in one "object file" do not affect the other's static state.
    tu_alpha::touch();
    tu_alpha::touch();
    tu_beta::touch();

    LEARN_CHECK(tu_alpha::hit_count() == 2);
    LEARN_CHECK(tu_beta::hit_count() == 1);

    // Exported functions can still be called across the link boundary.
    LEARN_CHECK(tu_alpha::exported_sum(10) == 12);
    LEARN_CHECK(tu_beta::exported_sum(10) == 11);
}

void demo_expert() {
    // Independence properties:
    // 1) Compile isolation — change beta.cpp → rebuild beta.o only (not alpha.o)
    // 2) State isolation — static / anonymous-ns data is per TU
    // 3) Interface coupling — only exported symbols form the contract at link
    //
    // Incremental builds rely on (1). ODR and undefined-ref issues live at (3).

    const int a_before = tu_alpha::hit_count();
    const int b_before = tu_beta::hit_count();

    for (int i = 0; i < 3; ++i) {
        tu_beta::touch();
    }

    LEARN_CHECK(tu_alpha::hit_count() == a_before);
    LEARN_CHECK(tu_beta::hit_count() == b_before + 3);

    // Distinct function identities for the two TUs' touch helpers.
    LEARN_CHECK(static_cast<void (*)()>(tu_alpha::touch) != static_cast<void (*)()>(tu_beta::touch));

    // Pair of object-file "products" after independent compilation.
    const auto objects = std::pair<std::string, std::string>{"alpha.o", "beta.o"};
    LEARN_CHECK(objects.first != objects.second);
    LEARN_CHECK(objects.first.ends_with(".o"));
    LEARN_CHECK(objects.second.ends_with(".o"));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage01/section03/per_cpp_object_file_independence", run>;

}  // namespace
