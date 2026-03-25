#include <gtest/gtest.h>

#include "ui/command.h"
#include "ui/menu_item.h"
#include "ui/menu.h"

using namespace fx;

// ---------------------------------------------------------------------------
// RelayCommand
// ---------------------------------------------------------------------------

TEST(RelayCommandTest, ExecutesFunction) {
    bool called = false;
    RelayCommand cmd([&]() { called = true; });
    cmd.execute();
    EXPECT_TRUE(called);
}

TEST(RelayCommandTest, CanExecuteDefaultsToTrue) {
    RelayCommand cmd([]() {});
    EXPECT_TRUE(cmd.can_execute());
}

TEST(RelayCommandTest, CanExecuteUsesProvidedPredicate) {
    bool allowed = false;
    RelayCommand cmd([]() {}, [&]() { return allowed; });
    EXPECT_FALSE(cmd.can_execute());
    allowed = true;
    EXPECT_TRUE(cmd.can_execute());
}

TEST(RelayCommandTest, NullExecuteFnDoesNotCrash) {
    RelayCommand cmd(nullptr);
    EXPECT_NO_THROW(cmd.execute());
}

// ---------------------------------------------------------------------------
// MenuItem — construction
// ---------------------------------------------------------------------------

TEST(MenuItemTest, ConstructsWithLabel) {
    MenuItem item("Open");
    EXPECT_EQ(item.label(), "Open");
    EXPECT_EQ(item.shortcut(), "");
    EXPECT_FALSE(item.is_separator());
    EXPECT_TRUE(item.is_enabled());
}

TEST(MenuItemTest, ConstructsWithShortcut) {
    MenuItem item("Open", nullptr, "Ctrl+O");
    EXPECT_EQ(item.shortcut(), "Ctrl+O");
}

TEST(MenuItemTest, SeparatorFactory) {
    auto sep = MenuItem::separator();
    EXPECT_TRUE(sep.is_separator());
    EXPECT_FALSE(sep.is_enabled());
}

// ---------------------------------------------------------------------------
// MenuItem — enabled flag
// ---------------------------------------------------------------------------

TEST(MenuItemTest, SetEnabledTogglesState) {
    MenuItem item("Close");
    item.set_enabled(false);
    EXPECT_FALSE(item.is_enabled());
    item.set_enabled(true);
    EXPECT_TRUE(item.is_enabled());
}

TEST(MenuItemTest, SeparatorCannotBeEnabled) {
    auto sep = MenuItem::separator();
    sep.set_enabled(true); // should be silently ignored
    EXPECT_FALSE(sep.is_enabled());
}

// ---------------------------------------------------------------------------
// MenuItem — can_trigger / trigger
// ---------------------------------------------------------------------------

TEST(MenuItemTest, CannotTriggerWithoutCommand) {
    MenuItem item("No Command");
    EXPECT_FALSE(item.can_trigger());
}

TEST(MenuItemTest, CannotTriggerWhenDisabled) {
    auto cmd = std::make_shared<RelayCommand>([]() {});
    MenuItem item("Item", cmd);
    item.set_enabled(false);
    EXPECT_FALSE(item.can_trigger());
}

TEST(MenuItemTest, CannotTriggerWhenCommandCannotExecute) {
    auto cmd = std::make_shared<RelayCommand>([]() {}, []() { return false; });
    MenuItem item("Item", cmd);
    EXPECT_FALSE(item.can_trigger());
}

TEST(MenuItemTest, CanTriggerWhenEnabledAndCommandAllows) {
    auto cmd = std::make_shared<RelayCommand>([]() {});
    MenuItem item("Item", cmd);
    EXPECT_TRUE(item.can_trigger());
}

TEST(MenuItemTest, TriggerExecutesCommand) {
    bool executed = false;
    auto cmd = std::make_shared<RelayCommand>([&]() { executed = true; });
    MenuItem item("Item", cmd);
    item.trigger();
    EXPECT_TRUE(executed);
}

TEST(MenuItemTest, TriggerIsNoOpWhenCannotTrigger) {
    bool executed = false;
    auto cmd = std::make_shared<RelayCommand>([&]() { executed = true; });
    MenuItem item("Item", cmd);
    item.set_enabled(false);
    item.trigger();
    EXPECT_FALSE(executed);
}

TEST(MenuItemTest, TriggerRespectsCanExecuteChanges) {
    bool allowed  = false;
    bool executed = false;
    auto cmd = std::make_shared<RelayCommand>(
        [&]() { executed = true; },
        [&]() { return allowed; }
    );
    MenuItem item("Item", cmd);

    item.trigger();
    EXPECT_FALSE(executed);

    allowed = true;
    item.trigger();
    EXPECT_TRUE(executed);
}

// ---------------------------------------------------------------------------
// Menu
// ---------------------------------------------------------------------------

TEST(MenuTest, ConstructsWithTitle) {
    Menu menu("File");
    EXPECT_EQ(menu.title(), "File");
    EXPECT_TRUE(menu.items().empty());
}

TEST(MenuTest, AddItem) {
    Menu menu("File");
    menu.add_item(MenuItem("Open"));
    ASSERT_EQ(menu.items().size(), 1u);
    EXPECT_EQ(menu.items()[0].label(), "Open");
}

TEST(MenuTest, AddSeparatorInsertsASeparatorItem) {
    Menu menu("File");
    menu.add_item(MenuItem("New"));
    menu.add_separator();
    menu.add_item(MenuItem("Exit"));
    ASSERT_EQ(menu.items().size(), 3u);
    EXPECT_TRUE(menu.items()[1].is_separator());
}

TEST(MenuTest, ItemsAreOrderedByInsertion) {
    Menu menu("Edit");
    menu.add_item(MenuItem("Cut"));
    menu.add_item(MenuItem("Copy"));
    menu.add_item(MenuItem("Paste"));
    EXPECT_EQ(menu.items()[0].label(), "Cut");
    EXPECT_EQ(menu.items()[1].label(), "Copy");
    EXPECT_EQ(menu.items()[2].label(), "Paste");
}

// ---------------------------------------------------------------------------
// Integration: Help menu with command binding
// ---------------------------------------------------------------------------

TEST(MenuIntegrationTest, HelpMenuOpensHelpWindowViaCommand) {
    bool help_opened = false;

    auto open_help = std::make_shared<RelayCommand>([&]() { help_opened = true; });

    Menu help("Help");
    help.add_item(MenuItem("Show Help", open_help, "F1"));

    ASSERT_EQ(help.items().size(), 1u);
    EXPECT_TRUE(help.items()[0].can_trigger());

    help.items()[0].trigger();
    EXPECT_TRUE(help_opened);
}

TEST(MenuIntegrationTest, DisabledHelpItemDoesNotOpenWindow) {
    bool help_opened = false;

    auto open_help = std::make_shared<RelayCommand>([&]() { help_opened = true; });

    Menu help("Help");
    MenuItem item("Show Help", open_help, "F1");
    item.set_enabled(false);
    help.add_item(std::move(item));

    help.items()[0].trigger();
    EXPECT_FALSE(help_opened);
}
