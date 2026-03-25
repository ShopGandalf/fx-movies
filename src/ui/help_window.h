#pragma once

#include "command.h"
#include <memory>
#include <string>

namespace fx {

// Overlay help window (modal-style).
//
// Owns its open/close state independently of any menu.
// Use open_command() to bind it to a MenuItem without any manual lambda:
//
//   Menu help("Help");
//   help.add_item(MenuItem("Show Help", help_window.open_command(), "F1"));
//
// Call render() once per ImGui frame — it is a no-op when the window is closed.
class HelpWindow {
public:
    explicit HelpWindow(std::string title = "Help");

    void open();
    void close();
    bool is_open() const;

    // Returns an ICommand that calls open() — bind directly to a MenuItem.
    std::shared_ptr<ICommand> open_command();

    // Draws the window via ImGui when is_open() is true.
    // No-op when closed.  Safe to call every frame.
    void render();

private:
    std::string title_;
    bool        is_open_ = false;
};

} // namespace fx
