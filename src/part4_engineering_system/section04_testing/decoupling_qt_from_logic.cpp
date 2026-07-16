// Doc      : 第4部分-工程系统.md
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : decoupling_qt_from_logic
// Topic id : part4/section04/decoupling_qt_from_logic

#include "learn/example_support.hpp"
#include "learn/topic_registry.hpp"

#include <string>
#include <string_view>

namespace {

struct DisplayModel {
    std::string text;
    bool warning{};
};

// This is the testable core: no QObject, signal, event loop, or widget type.
class TemperaturePresenter {
public:
    [[nodiscard]] DisplayModel present(int celsius) const {
        return {std::to_string(celsius) + " C", celsius >= warning_threshold_};
    }

private:
    int warning_threshold_{80};
};

class DisplayPort {
public:
    virtual ~DisplayPort() = default;
    virtual void render(const DisplayModel& model) = 0;
};

class DisplayController {
public:
    explicit DisplayController(DisplayPort& view) : view_(view) {}
    void on_temperature_changed(int celsius) { view_.render(presenter_.present(celsius)); }

private:
    DisplayPort& view_;
    TemperaturePresenter presenter_;
};

class FakeDisplay final : public DisplayPort {
public:
    void render(const DisplayModel& model) override {
        last_text = model.text;
        warning_visible = model.warning;
        ++render_count;
    }

    std::string last_text;
    bool warning_visible{};
    int render_count{};
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;
    ::learn::ExampleChecks checks{"part4/section04/decoupling_qt_from_logic"};

    TemperaturePresenter core;
    LEARN_EXPECT_EQ(checks, core.present(21).text, std::string{"21 C"});
    LEARN_EXPECT(checks, !core.present(79).warning);
    LEARN_EXPECT(checks, core.present(80).warning);

    FakeDisplay fake;
    DisplayController controller{fake};
    controller.on_temperature_changed(95);
    LEARN_EXPECT_EQ(checks, fake.last_text, std::string{"95 C"});
    LEARN_EXPECT(checks, fake.warning_visible);
    LEARN_EXPECT_EQ(checks, fake.render_count, 1);

    constexpr std::string_view production_adapter = "Qt adapter converts DisplayModel into QLabel/QObject operations";
    LEARN_EXPECT(checks, production_adapter.contains("adapter"));

    return checks.result();
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/decoupling_qt_from_logic", run>;

}  // namespace
