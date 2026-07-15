// LearnCpp topic example
// Doc      : part2-stage09-exceptions-error-handling-ub.md
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section05_non_exception_error_styles
// Item     : std_error_code_category
// Topic id : part2/stage09/section05/std_error_code_category
//
// Covers: std::error_code + custom error_category

#include "learn/topic_registry.hpp"

#include <string>
#include <system_error>

namespace {

enum class AppErr { Ok = 0, NotFound = 1, Busy = 2 };

struct AppCategory : std::error_category {
    const char* name() const noexcept override { return "app"; }
    std::string message(int ev) const override {
        switch (static_cast<AppErr>(ev)) {
            case AppErr::Ok:
                return "ok";
            case AppErr::NotFound:
                return "not found";
            case AppErr::Busy:
                return "busy";
        }
        return "unknown";
    }
};

const AppCategory& app_category() {
    static AppCategory cat;
    return cat;
}

std::error_code make_app_error(AppErr e) {
    return {static_cast<int>(e), app_category()};
}

void demo_basics() {
    std::error_code ec = make_app_error(AppErr::NotFound);
    LEARN_CHECK(ec);
    LEARN_CHECK(ec.value() == 1);
    LEARN_CHECK(std::string{ec.category().name()} == "app");
}

void demo_intermediate() {
    auto ec = make_app_error(AppErr::Ok);
    LEARN_CHECK(!ec);  // value 0 => success in boolean context for error_code
    LEARN_CHECK(ec.message() == "ok");
}

void demo_expert() {
    auto a = make_app_error(AppErr::Busy);
    auto b = make_app_error(AppErr::Busy);
    LEARN_CHECK(a == b);
    LEARN_CHECK(a.message() == "busy");

    std::error_code generic = std::make_error_code(std::errc::invalid_argument);
    LEARN_CHECK(generic == std::errc::invalid_argument);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    demo_basics();
    demo_intermediate();
    demo_expert();
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section05/std_error_code_category", run>;

}  // namespace
