#pragma once

#include "menu_item.h"
#include <string>
#include <vector>

namespace fx {

// A named drop-down inside a menu bar (e.g. "File", "Help").
// Items are ordered; call add_separator() to insert a visual divider.
//
// Example:
//   Menu help("Help");
//   help.add_item(MenuItem("Show Help", open_help_cmd, "F1"));
//   help.add_item(MenuItem("About",     about_cmd));
class Menu {
public:
    explicit Menu(std::string title);

    const std::string&        title() const;
    const std::vector<MenuItem>& items() const;

    void add_item(MenuItem item);
    void add_separator();

private:
    std::string           title_;
    std::vector<MenuItem> items_;
};

} // namespace fx
