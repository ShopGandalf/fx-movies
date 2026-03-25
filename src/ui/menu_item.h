#pragma once

#include "command.h"
#include <memory>
#include <string>

namespace fx {

// A single entry inside a Menu.
//
// Can be a regular item (label + optional command + optional keyboard shortcut)
// or a visual separator (use MenuItem::separator()).
//
// The attached ICommand follows the WPF command pattern:
//   - can_trigger() reflects ICommand::can_execute() and the enabled flag
//   - trigger()     calls ICommand::execute() only when can_trigger() is true
class MenuItem {
public:
    // Factory for separator items.
    static MenuItem separator();

    // Regular item.  command may be nullptr for display-only entries.
    explicit MenuItem(std::string label,
                      std::shared_ptr<ICommand> command = nullptr,
                      std::string shortcut = "");

    const std::string& label()    const;
    const std::string& shortcut() const;
    bool               is_separator() const;
    bool               is_enabled()   const;
    void               set_enabled(bool enabled);

    // Returns true when the item is enabled, not a separator, has a command,
    // and the command's can_execute() is true.
    bool can_trigger() const;

    // Executes the command if can_trigger() is true.  No-op otherwise.
    void trigger() const;

private:
    MenuItem(); // separator constructor

    std::string              label_;
    std::string              shortcut_;
    std::shared_ptr<ICommand> command_;
    bool                     enabled_      = true;
    bool                     is_separator_ = false;
};

} // namespace fx
