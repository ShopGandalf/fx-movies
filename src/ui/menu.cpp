#include "ui/menu.h"

namespace fx {

Menu::Menu(std::string title) : title_(std::move(title)) {}

const std::string&           Menu::title() const { return title_; }
const std::vector<MenuItem>& Menu::items() const { return items_; }

void Menu::add_item(MenuItem item) {
    items_.push_back(std::move(item));
}

void Menu::add_separator() {
    items_.push_back(MenuItem::separator());
}

} // namespace fx
