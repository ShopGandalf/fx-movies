#include "ui/menu_item.h"

namespace fx {

MenuItem MenuItem::separator() {
    return MenuItem{};
}

MenuItem::MenuItem()
    : enabled_(false)
    , is_separator_(true)
{}

MenuItem::MenuItem(std::string label,
                   std::shared_ptr<ICommand> command,
                   std::string shortcut)
    : label_(std::move(label))
    , shortcut_(std::move(shortcut))
    , command_(std::move(command))
{}

const std::string& MenuItem::label()    const { return label_; }
const std::string& MenuItem::shortcut() const { return shortcut_; }
bool               MenuItem::is_separator() const { return is_separator_; }
bool               MenuItem::is_enabled()   const { return enabled_; }

void MenuItem::set_enabled(bool enabled) {
    if (!is_separator_) enabled_ = enabled;
}

bool MenuItem::can_trigger() const {
    return enabled_
        && !is_separator_
        && command_ != nullptr
        && command_->can_execute();
}

void MenuItem::trigger() const {
    if (can_trigger()) command_->execute();
}

} // namespace fx
